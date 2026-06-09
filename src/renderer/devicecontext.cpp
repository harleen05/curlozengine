/*
 * @file devicecontext.cpp
 * @author curl0z
 * @brief Implementation of the initialization
 * and cleanup of device context
 */
#include "renderer/devicecontext.hpp"
#include "core/logs.hpp"
#include "renderer/variables.hpp"
#include <algorithm>
#include <config/config.hpp>
#include <cstring>
#include <vector>
#include <vulkan/vulkan.h>
#include <window/vulkanhelper.hpp>

namespace clz::renderer
{
#ifdef NDEBUG
	constexpr bool enableValidationLayers = false;
#else
	constexpr bool enableValidationLayers = true;
#endif

	std::expected<void, std::string>
	getRequiredInstanceExtensions(std::vector<const char*>& rRequiredExtensions)
	{
		auto result = clz::window::getRequiredVulkanExtensions(rRequiredExtensions);
		if (!result)
		{
			return std::unexpected(result.error());
		}

		if (enableValidationLayers)
		{
			rRequiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		uint32_t count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(count);
		vkEnumerateInstanceExtensionProperties(nullptr, &count, availableExtensions.data());

		for (auto requiredExtension : rRequiredExtensions)
		{
			const bool found =
			    std::any_of(availableExtensions.begin(), availableExtensions.end(),
					[&](auto availableExtension) {
						return std::strcmp(availableExtension.extensionName,
								   requiredExtension) == 0;
					});
			if (!found)
				return std::unexpected(
				    "Extension: " + std::string(requiredExtension) +
				    " not available");
		}
		clz::log::debug("All required instance extensions present");
		return {};
	}

	std::expected<void, std::string>
	getValidationLayers(std::vector<const char*>& rValidationLayers)
	{
		rValidationLayers = {"VK_LAYER_KHRONOS_validation"};

		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (auto layer : rValidationLayers)
		{
			const bool found = std::any_of(
			    availableLayers.begin(), availableLayers.end(),
			    [&](auto availableLayer) {
				    return std::strcmp(layer, availableLayer.layerName) == 0;
			    });
			if (!found)
			{
				return std::unexpected("Layer: " + std::string(layer) +
						       " not available");
			}
		}

		clz::log::debug("All requested layers present");
		return {};
	}

	std::expected<void, std::string> createInstance()
	{
		// Application Info
		VkApplicationInfo appInfo = {};

		// Define which type of object is this
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		// Name of our application
		const std::string appName = clz::config::getAppName();
		appInfo.pApplicationName = appName.c_str();
		// Our application's version
		appInfo.applicationVersion =
		    VK_MAKE_VERSION(clz::config::getInt("engine", "version_major", 0),
				    clz::config::getInt("engine", "version_minor", 0),
				    clz::config::getInt("engine", "version_major", 0));
		// Our application's name
		appInfo.pEngineName = "Curloz Engine";
		// Which API version we want to use
		appInfo.apiVersion = VK_API_VERSION_1_3;

		// Instance info
		VkInstanceCreateInfo instanceInfo = {};

		// Struct type
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		// Our application's info
		instanceInfo.pApplicationInfo = &appInfo;

		// Get Extensions
		std::vector<const char*> requiredExtensions;
		auto instanceResult = getRequiredInstanceExtensions(requiredExtensions);
		if (!instanceResult)
			return std::unexpected(instanceResult.error());

		// Send info about extensions
		instanceInfo.enabledExtensionCount = requiredExtensions.size();
		instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();

		// Enable layers
		std::vector<const char*> layers;
		auto layersResult = getValidationLayers(layers);
		if (!layersResult)
			return std::unexpected(layersResult.error());

		if (enableValidationLayers)
		{
			instanceInfo.enabledLayerCount = layers.size();
			instanceInfo.ppEnabledLayerNames = layers.data();
		}
		else
		{
			instanceInfo.enabledLayerCount = 0;
			instanceInfo.ppEnabledLayerNames = nullptr;
		}

		VkResult instanceCreationResult =
		    vkCreateInstance(&instanceInfo, nullptr, &r_deviceContext.instance);
		if (instanceCreationResult != VK_SUCCESS)
		{
			clz::log::error("Could not create instance");
			return std::unexpected("Could not create instance");
		}

		clz::log::debug("renderer: created instance");
		return {};
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL
	printMessage(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		     VkDebugUtilsMessageTypeFlagsEXT messageType,
		     const VkDebugUtilsMessengerCallbackDataEXT* pMessageData, void* pUserData)
	{
		switch (messageSeverity)
		{
		case (VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT):
			clz::log::info("VULKAN VALIDATION LAYERS: " +
				       std::string(pMessageData->pMessage));
			break;
		case (VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT):
			clz::log::warn("VULKAN VALIDATION LAYERS: " +
				       std::string(pMessageData->pMessage));
			break;
		case (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT):
			clz::log::error("VULKAN VALIDATION LAYERS: " +
					std::string(pMessageData->pMessage));
			break;
		default:
			break;
		}
		return VK_FALSE;
	}

	std::expected<void, std::string> createDebugMessenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT messengerInfo{};
		messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
						VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
						VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
					    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
					    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		messengerInfo.pfnUserCallback = printMessage;
		messengerInfo.pUserData = nullptr; // Optional

		const auto createMessenger =
		    reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
			r_deviceContext.instance, "vkCreateDebugUtilsMessengerEXT"));
		if (createMessenger == nullptr)
		{
			clz::log::error(
			    "renderer: Could not find the debug messenger creator function");
			return std::unexpected(
			    "Could not find the debug messenger creator function");
		}

		if (createMessenger(r_deviceContext.instance, &messengerInfo, nullptr,
				    &r_deviceContext.debugMessenger) != VK_SUCCESS)
		{
			clz::log::error("renderer: Could not create debug messenger");
			return std::unexpected("could not create debug messenger");
		}
		clz::log::debug("renderer: Created Debug Messenger");

		return {};
	}

	std::expected<void, std::string> createSurface()
	{
		auto result = clz::window::createVulkanSurface(r_deviceContext.instance,
							       r_deviceContext.surface);
		if (!result)
			return std::unexpected(result.error());

		clz::log::debug("Created window surface");
		return {};
	}

	std::expected<void, std::string> selectPhysicalDevice()
	{
		uint32_t numPhysicalDevices = 0;
		vkEnumeratePhysicalDevices(r_deviceContext.instance, &numPhysicalDevices, nullptr);
		std::vector<VkPhysicalDevice> physicalDevices(numPhysicalDevices);
		vkEnumeratePhysicalDevices(r_deviceContext.instance, &numPhysicalDevices,
					   physicalDevices.data());

		uint32_t maxScore = 0;
		for (auto physicalDevice : physicalDevices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
			if (deviceProperties.apiVersion < VK_API_VERSION_1_3)
			{
				clz::log::warn(
				    "Physical Device: " + std::string(deviceProperties.deviceName) +
				    ", Does not support vulkan 1.3");
				continue;
			}

			// check graphics support
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,
								 nullptr);
			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,
								 queueFamilies.data());

			bool graphicsSupport = false;
			bool presentSupport = false;
			for (int j = 0; j < queueFamilies.size(); ++j)
			{
				// Graphics Support
				if (!graphicsSupport &&
				    queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					r_deviceContext.graphicsFamily = j;
					graphicsSupport = true;
				}

				// Present Support
				if (!presentSupport)
				{
					VkBool32 presentFamilyFound = VK_FALSE;
					vkGetPhysicalDeviceSurfaceSupportKHR(
					    physicalDevice, j, r_deviceContext.surface,
					    &presentFamilyFound);
					if (presentFamilyFound == VK_TRUE)
					{
						r_deviceContext.presentFamily = j;
						presentSupport = true;
					}
				}
				if (graphicsSupport && presentSupport)
					break;
			}

			if (!graphicsSupport || !presentSupport)
			{
				clz::log::warn("GPU: " + std::string(deviceProperties.deviceName) +
					       "is not suitable");
				continue;
			}

			// this device's local score
			uint32_t deviceScore = 0;

			// First parameter - Checking for maximum image size
			deviceScore += deviceProperties.limits.maxImageDimension2D;

			// Checking if the device is Discrete or not
			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				deviceScore += 10000;

			// Final comparison with previous devices to see if current device has a
			// better score
			if (deviceScore > maxScore)
			{
				maxScore = deviceScore;
				r_deviceContext.gpu = physicalDevice;
			}
		}

		if (r_deviceContext.gpu == VK_NULL_HANDLE)
		{
			clz::log::error("No Suitable GPU found");
			return std::unexpected("No Suitable GPU found");
		}
		VkPhysicalDeviceProperties selecrtedDeviceProperties;
		vkGetPhysicalDeviceProperties(r_deviceContext.gpu, &selecrtedDeviceProperties);

		clz::log::debug("Using GPU: " + std::string(selecrtedDeviceProperties.deviceName));

		return {};
	}

	std::expected<void, std::string>
	getRequiredDeviceExtensions(std::vector<const char*>& rRequiredDeviceExtensions)
	{
		// enable swapchain extension
		rRequiredDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

		uint32_t deviceExtensionCount = 0;
		vkEnumerateDeviceExtensionProperties(r_deviceContext.gpu, nullptr,
						     &deviceExtensionCount, nullptr);
		std::vector<VkExtensionProperties> availableDeviceExtensions(deviceExtensionCount);
		vkEnumerateDeviceExtensionProperties(r_deviceContext.gpu, nullptr,
						     &deviceExtensionCount,
						     availableDeviceExtensions.data());

		for (auto requiredDeviceExtension : rRequiredDeviceExtensions)
		{
			const bool found = std::any_of(
			    availableDeviceExtensions.begin(), availableDeviceExtensions.end(),
			    [&](auto availableExtension) {
				    return std::strcmp(requiredDeviceExtension,
						       availableExtension.extensionName) == 0;
			    });
			if (!found)
			{
				clz::log::error("required vulkan device extension: " +
						std::string(requiredDeviceExtension) +
						" is not supported by the selected GPU");
				return std::unexpected("required vulkan device extension: " +
						       std::string(requiredDeviceExtension) +
						       " is not supported by the selected GPU");
			}
		}

		clz::log::debug("All device extensions present");
		return {};
	}

	std::expected<void, std::string> createLogicalDevice()
	{
		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		// vector holding all the queue infos
		std::vector<VkDeviceQueueCreateInfo> queueInfos;

		constexpr float queuePriority = 1.0f;
		if (r_deviceContext.graphicsFamily == r_deviceContext.presentFamily)
		{
			VkDeviceQueueCreateInfo queueInfo = {};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = r_deviceContext.graphicsFamily.value();
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &queuePriority;
			queueInfos.push_back(queueInfo);
		}
		else
		{
			VkDeviceQueueCreateInfo graphicsQueueInfo = {};
			graphicsQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			graphicsQueueInfo.queueFamilyIndex = r_deviceContext.graphicsFamily.value();
			graphicsQueueInfo.queueCount = 1;
			graphicsQueueInfo.pQueuePriorities = &queuePriority;

			VkDeviceQueueCreateInfo presentQueueInfo = {};
			presentQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			presentQueueInfo.queueFamilyIndex = r_deviceContext.presentFamily.value();
			presentQueueInfo.queueCount = 1;
			presentQueueInfo.pQueuePriorities = &queuePriority;

			queueInfos.push_back(graphicsQueueInfo);
			queueInfos.push_back(presentQueueInfo);
		}

		deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
		deviceInfo.pQueueCreateInfos = queueInfos.data();

		// Device Features — enable dynamic rendering
		VkPhysicalDeviceVulkan13Features features13 = {};
		features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		features13.dynamicRendering = VK_TRUE;
		features13.synchronization2 = VK_TRUE;
		features13.pNext = nullptr;

		deviceInfo.pNext = &features13;

		std::vector<const char*> requiredDeviceExtensions;
		getRequiredDeviceExtensions(requiredDeviceExtensions);

		deviceInfo.enabledExtensionCount =
		    static_cast<uint32_t>(requiredDeviceExtensions.size());
		deviceInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();

		deviceInfo.pEnabledFeatures = nullptr;

		if (vkCreateDevice(r_deviceContext.gpu, &deviceInfo, nullptr,
				   &r_deviceContext.device) != VK_SUCCESS)
		{
			clz::log::error("Failed to create renderer's logical device");
			return std::unexpected("Failed to create renderer's logical device");
		}
		clz::log::debug("created vulkan logical device");

		vkGetDeviceQueue(r_deviceContext.device, r_deviceContext.graphicsFamily.value(), 0,
				 &r_deviceContext.graphicsQueue);
		vkGetDeviceQueue(r_deviceContext.device, r_deviceContext.presentFamily.value(), 0,
				 &r_deviceContext.presentQueue);

		clz::log::debug("created logical device");

		return {};
	}

	void destroyInstance()
	{
		vkDestroyInstance(r_deviceContext.instance, nullptr);
		clz::log::debug("Destroyed vulkan instance");
	}

	void destroyDebugMessenger()
	{
		auto destroyMessenger =
		    reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
			r_deviceContext.instance, "vkDestroyDebugUtilsMessengerEXT"));
		if (destroyMessenger != nullptr)
		{
			destroyMessenger(r_deviceContext.instance, r_deviceContext.debugMessenger,
					 nullptr);
		}
		clz::log::debug("Destroyed debug messenger");
	}

	void destroySurface()
	{
		vkDestroySurfaceKHR(r_deviceContext.instance, r_deviceContext.surface, nullptr);
		clz::log::debug("Destroyed surface instance");
	}

	void destroyDevice()
	{
		vkDestroyDevice(r_deviceContext.device, nullptr);
		clz::log::debug("Destroyed logical device");
	}
} // namespace clz::renderer