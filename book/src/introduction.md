# Puara Module

**Société des Arts Technologiques (SAT)**  
**Input Devices and Music Interaction Laboratory (IDMIL)**

---

## Overview

Puara Module is a library for ESP32 boards that provides a simplified interface for managing WiFi, a web server, and a filesystem. It facilitates the creation of distributed interactive systems, networked controllers, and similar applications, allowing users to focus on prototyping the rest of their system.

---

## Getting Started

This documentation is divided into two main sections:

### 1. Building Puara Module Templates/Examples

Choose your preferred development environment. We offer almost identical templates for both Arduino IDE and PlatformIO.

| Environment | Recommended For |
|-------------|-----------------|
| [Arduino IDE](arduino_user.md) | Beginner to intermediate programmers |
| [PlatformIO](pio_user.md) | Intermediate to advanced programmers, or users wanting more control |

Once your project is running, use the [Browser Pages](browser.md) to configure your device's network settings and custom variables.

### 2. Developer Reference

Technical documentation for understanding and extending the codebase:

- [Puara API](puara.md) - Main `Puara` object and its methods
- [Filesystem](filesystem.md) - LittleFS vs SPIFFS configuration
- [Web Server](web.md) - Integrated web server for device configuration
- [WiFi](wifi.md) - WiFi connection management
- [Serial](serial.md) - Serial communication and debug output
- [mDNS](mdns.md) - Local network device discovery
