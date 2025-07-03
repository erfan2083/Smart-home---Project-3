#ifndef HTML_PAGE_H
#define HTML_PAGE_H
//#include <Arduino.h>
#include <pgmspace.h>

const char smartHomeHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>  
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EFN Smart House</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <style>
        body {
            font-family: 'Montserrat', sans-serif;
            background: linear-gradient(135deg, #1a2a6c, #b21f1f, #fdbb2d);
            background-size: 600% 600%;
            animation: gradientBG 15s ease infinite;
            min-height: 100vh;
            margin: 0;
            padding: 0;
        }
        
        @keyframes gradientBG {
            0% { background-position: 0% 50% }
            50% { background-position: 100% 50% }
            100% { background-position: 0% 50% }
        }
        
        .glass {
            background: rgba(255, 255, 255, 0.15);
            backdrop-filter: blur(10px);
            border-radius: 20px;
            border: 1px solid rgba(255, 255, 255, 0.2);
            box-shadow: 0 8px 32px 0 rgba(31, 38, 135, 0.37);
        }
        
        .room {
            position: relative;
            transition: all 0.5s ease;
            overflow: hidden;
        }
        
        .room:hover {
            transform: translateY(-10px);
            box-shadow: 0 15px 30px rgba(0, 0, 0, 0.3);
        }
        
        .light {
            width: 20px;
            height: 20px;
            border-radius: 50%;
            position: absolute;
            transition: all 0.3s ease;
            filter: drop-shadow(0 0 4px rgba(255, 255, 255, 0.5));
        }
        
        .light.on {
            background-color: #ffee58;
            box-shadow: 0 0 15px 5px rgba(255, 244, 88, 0.7);
        }
        
        .light.off {
            background-color: #78909c;
        }
        
        .ac {
            width: 30px;
            height: 15px;
            position: absolute;
            transition: all 0.3s ease;
        }
        
        .ac.on {
            background-color: #81d4fa;
            box-shadow: 0 0 10px 2px rgba(129, 212, 250, 0.7);
        }
        
        .ac.off {
            background-color: #78909c;
        }
        
        .room-icon {
            font-size: 2.5rem;
            margin-bottom: 0.5rem;
        }
        
        .pulse {
            animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
            0% { transform: scale(1); }
            50% { transform: scale(1.1); }
            100% { transform: scale(1); }
        }
        
        .floating {
            animation: floating 3s ease-in-out infinite;
        }
        
        @keyframes floating {
            0% { transform: translateY(0px); }
            50% { transform: translateY(-15px); }
            100% { transform: translateY(0px); }
        }
        
        .apartment-container {
            position: relative;
            width: 100%;
            max-width: 800px;
            height: 500px;
            margin: 0 auto;
        }
        
        .apartment {
            width: 100%;
            height: 100%;
            position: relative;
            background-color: rgba(255, 255, 255, 0.1);
            border-radius: 20px;
            overflow: hidden;
            border: 2px solid rgba(255, 255, 255, 0.3);
        }
        
        .room-area {
            position: absolute;
            border: 2px solid rgba(255, 255, 255, 0.5);
            border-radius: 10px;
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            cursor: pointer;
            transition: all 0.3s ease;
            background-color: rgba(255, 255, 255, 0.1);
            overflow: hidden;
        }
        
        .room-area:hover {
            background-color: rgba(255, 255, 255, 0.25);
            transform: scale(1.02);
        }
        
        .room-area::before {
            content: '';
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: linear-gradient(45deg, transparent, rgba(255, 255, 255, 0.1), transparent);
            transition: all 0.5s ease;
        }
        
        .room-area:hover::before {
            transform: translateX(100%);
        }
        
        .room-label {
            font-weight: bold;
            color: white;
            text-shadow: 0 2px 4px rgba(0, 0, 0, 0.5);
            z-index: 10;
            font-size: 1.2rem;
        }
        
        .device-icon {
            position: absolute;
            width: 24px;
            height: 24px;
            border-radius: 50%;
            background-color: rgba(255, 255, 255, 0.2);
            display: flex;
            justify-content: center;
            align-items: center;
            font-size: 12px;
            color: white;
        }
        
        .status-badge {
            position: absolute;
            top: 10px;
            right: 10px;
            padding: 5px 10px;
            border-radius: 20px;
            font-size: 0.8rem;
            font-weight: bold;
            z-index: 10;
        }
        
        .status-ok {
            background-color: rgba(72, 187, 120, 0.7);
            color: white;
        }
        
        .status-warning {
            background-color: rgba(237, 137, 54, 0.7);
            color: white;
        }
        
        .logo-container {
            position: relative;
            width: 120px;
            height: 120px;
            margin: 0 auto;
        }
        
        .logo-bg {
            position: absolute;
            width: 100%;
            height: 100%;
            background: linear-gradient(135deg, #6e8efb, #a777e3);
            border-radius: 50%;
            box-shadow: 0 8px 32px rgba(31, 38, 135, 0.4);
            animation: rotateBg 8s linear infinite;
        }
        
        @keyframes rotateBg {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
        
        .logo-inner {
            position: absolute;
            width: 80%;
            height: 80%;
            top: 10%;
            left: 10%;
            background: white;
            border-radius: 50%;
            display: flex;
            justify-content: center;
            align-items: center;
            font-weight: bold;
            font-size: 2rem;
            color: #4c1d95;
            z-index: 2;
        }
        
        .nav-button {
            transition: all 0.3s ease;
        }
        
        .nav-button:hover {
            transform: translateY(-5px);
        }
        
        /* Stats cards */
        .stat-card {
            position: relative;
            overflow: hidden;
            transition: all 0.3s ease;
        }
        
        .stat-card:hover {
            transform: translateY(-5px);
        }
        
        .stat-icon {
            font-size: 2rem;
            margin-bottom: 0.5rem;
        }
        
        .stat-value {
            font-size: 2.5rem;
            font-weight: bold;
            margin-bottom: 0.5rem;
        }
        
        .stat-label {
            font-size: 1rem;
            opacity: 0.8;
        }
        
        /* Circular progress */
        .circular-progress {
            position: relative;
            width: 120px;
            height: 120px;
        }
        
        .circular-progress svg {
            width: 100%;
            height: 100%;
            transform: rotate(-90deg);
        }
        
        .circular-progress circle {
            fill: none;
            stroke-width: 8;
            stroke-linecap: round;
        }
        
        .circular-progress .bg {
            stroke: rgba(255, 255, 255, 0.2);
        }
        
        .circular-progress .progress {
            stroke: white;
            transition: stroke-dashoffset 0.5s ease;
        }
        
        .circular-progress .value {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            font-size: 1.5rem;
            font-weight: bold;
            color: white;
        }
        
        /* Local storage for room states */
        #local-storage {
            display: none;
        }
    </style>
    <link href="https://fonts.googleapis.com/css2?family=Montserrat:wght@300;400;500;600;700&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css">
</head>
<body>
    <!-- Hidden local storage for room states -->
    <div id="local-storage" data-bedroom-light1="off" data-bedroom-light2="off" data-bedroom-auto="false"
         data-bathroom-light="off" data-bathroom-auto="true"
         data-livingroom-light1="off" data-livingroom-light2="off" data-livingroom-light3="off" 
         data-livingroom-ac="off" data-livingroom-lights-auto="false" data-livingroom-ac-auto="false"
         data-temperature="23.5" data-humidity="45" data-energy="78">
    </div>

    <!-- Main Page -->
    <div id="main-page" class="min-h-screen flex flex-col items-center p-6 md:p-10">
        <!-- Logo -->
        <div class="mb-8 text-center">
            <div class="logo-container">
                <div class="logo-bg"></div>
                <div class="logo-inner">EFN</div>
            </div>
            <h2 class="text-2xl font-bold text-white mt-4 tracking-wider">SMART HOME</h2>
            <p class="text-white text-opacity-80 mt-1">Control your living space</p>
        </div>

        <!-- Stats Cards -->
        <div class="w-full max-w-4xl mb-10">
            <div class="grid grid-cols-1 md:grid-cols-3 gap-6">
                <!-- Temperature Card -->
                <div class="stat-card glass p-6 flex flex-col items-center">
                    <div class="stat-icon text-white">
                        <i class="fas fa-temperature-high"></i>
                    </div>
                    <div class="stat-value text-white" id="temperature-value">24.0°C</div>
                    <div class="stat-label text-white">Temperature</div>
                    <div class="w-full bg-white bg-opacity-20 h-2 rounded-full mt-4 overflow-hidden">
                        <div id="temperature-bar" class="bg-gradient-to-r from-blue-300 to-red-500 h-full rounded-full" style="width: 60%"></div>
                    </div>
                    <div class="flex justify-between w-full mt-2 text-xs text-white">
                        <span>-40°C</span>
                        <span>80°C</span>
                    </div>
                </div>
                
                <!-- Humidity Card -->
                <div class="stat-card glass p-6 flex flex-col items-center">
                    <div class="stat-icon text-white">
                        <i class="fas fa-tint"></i>
                    </div>
                    <div class="circular-progress">
                        <svg>
                            <circle class="bg" cx="60" cy="60" r="54" stroke-dasharray="339.292" stroke-dashoffset="0"></circle>
                            <circle id="humidity-circle" class="progress" cx="60" cy="60" r="54" stroke-dasharray="339.292" stroke-dashoffset="169.646"></circle>
                        </svg>
                        <div class="value" id="humidity-value">40%</div>
                    </div>
                    <div class="stat-label text-white mt-2">Humidity</div>
                </div>
                
                <!-- Energy Usage Card -->
                <div class="stat-card glass p-6 flex flex-col items-center">
                    <div class="stat-icon text-white">
                        <i class="fas fa-bolt"></i>
                    </div>
                    <div class="stat-value text-white" id="energy-value">78W</div>
                    <div class="stat-label text-white">Current Usage</div>
                    <div class="w-full mt-4">
                        <div class="flex justify-between text-xs text-white mb-1">
                            <span>Low</span>
                            <span>High</span>
                        </div>
                        <div class="w-full bg-white bg-opacity-20 h-2 rounded-full overflow-hidden">
                            <div id="energy-bar" class="bg-gradient-to-r from-green-400 to-yellow-500 h-full rounded-full" style="width: 78%"></div>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!-- Apartment Overview -->
        <div class="apartment-container mb-10">
            <div class="apartment">
                <!-- Bedroom -->
                <div id="bedroom-area" class="room-area" style="top: 0; left: 0; width: 40%; height: 60%;" onclick="navigateTo('bedroom-page')">
                    <span class="room-label">Bedroom</span>
                    <div class="status-badge status-ok">2 Lights</div>
                    <i class="fas fa-bed text-white text-opacity-50 text-4xl mt-4"></i>
                    
                    <div class="device-icon" style="top: 30%; left: 30%;">
                        <i class="fas fa-lightbulb"></i>
                    </div>
                    <div class="device-icon" style="top: 70%; left: 70%;">
                        <i class="fas fa-lightbulb"></i>
                    </div>
                </div>
                
                <!-- Bathroom -->
                <div id="bathroom-area" class="room-area" style="top: 0; right: 0; width: 30%; height: 40%;" onclick="navigateTo('bathroom-page')">
                    <span class="room-label">Bathroom</span>
                    <div class="status-badge status-ok">Auto</div>
                    <i class="fas fa-bath text-white text-opacity-50 text-4xl mt-4"></i>
                    
                    <div class="device-icon" style="top: 50%; left: 50%;">
                        <i class="fas fa-lightbulb"></i>
                    </div>
                </div>
                
                <!-- Living Room -->
                <div id="living-room-area" class="room-area" style="bottom: 0; right: 0; width: 60%; height: 60%;" onclick="navigateTo('living-room-page')">
                    <span class="room-label">Living Room</span>
                    <div class="status-badge status-ok">3 Lights + AC</div>
                    <i class="fas fa-couch text-white text-opacity-50 text-4xl mt-4"></i>
                    
                    <div class="device-icon" style="top: 30%; left: 20%;">
                        <i class="fas fa-lightbulb"></i>
                    </div>
                    <div class="device-icon" style="top: 30%; left: 50%;">
                        <i class="fas fa-lightbulb"></i>
                    </div>
                    <div class="device-icon" style="top: 30%; left: 80%;">
                        <i class="fas fa-lightbulb"></i>
                    </div>
                    <div class="device-icon" style="top: 70%; left: 50%;">
                        <i class="fas fa-wind"></i>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Bedroom Page -->
    <div id="bedroom-page" class="min-h-screen flex flex-col items-center p-6 md:p-10 hidden">
        <!-- Header -->
        <div class="w-full max-w-3xl flex items-center mb-8">
            <button class="back-btn mr-4 text-white text-2xl" onclick="navigateTo('main-page')">
                <i class="fas fa-arrow-left"></i>
            </button>
            <div>
                <h2 class="text-2xl font-bold text-white">Bedroom</h2>
                <p class="text-white text-opacity-70">Control your bedroom devices</p>
            </div>
        </div>

        <!-- Room Visualization -->
        <div class="w-full max-w-3xl glass p-6 rounded-xl mb-6">
            <h3 class="text-xl font-semibold text-white mb-4">Room Overview</h3>
            <div class="relative w-full h-64 bg-indigo-900 bg-opacity-30 rounded-lg border border-white border-opacity-20">
                <div id="bedroom-light-1" class="light off" style="top: 30%; left: 30%"></div>
                <div id="bedroom-light-2" class="light off" style="top: 70%; left: 70%"></div>
                
                <div class="absolute top-2 left-2 text-white text-opacity-80">
                    <i class="fas fa-bed text-2xl"></i>
                </div>
                
                <div class="absolute bottom-2 right-2 text-white text-opacity-80">
                    <i class="fas fa-door-open text-2xl"></i>
                </div>
            </div>
        </div>

        <!-- Controls -->
        <div class="w-full max-w-3xl glass p-6 rounded-xl">
            <h3 class="text-xl font-semibold text-white mb-4">Light Controls</h3>
            
            <div class="mb-6 p-4 bg-white bg-opacity-10 rounded-lg">
                <div class="flex items-center justify-between mb-4">
                    <span class="text-white">Light 1</span>
                    <button id="bedroom-light-1-btn" class="px-4 py-2 bg-gray-600 text-white rounded-md">OFF</button>
                </div>
                
                <div class="flex items-center justify-between">
                    <span class="text-white">Light 2</span>
                    <button id="bedroom-light-2-btn" class="px-4 py-2 bg-gray-600 text-white rounded-md">OFF</button>
                </div>
            </div>
            
            <div class="flex items-center justify-between p-4 bg-white bg-opacity-10 rounded-lg">
                <span class="text-white">Automatic Mode</span>
                <label class="relative inline-flex items-center cursor-pointer">
                    <input type="checkbox" id="bedroom-auto" class="sr-only peer">
                    <div class="w-14 h-7 bg-gray-600 peer-focus:outline-none rounded-full peer peer-checked:after:translate-x-full peer-checked:after:border-white after:content-[''] after:absolute after:top-0.5 after:left-[4px] after:bg-white after:border-gray-300 after:border after:rounded-full after:h-6 after:w-6 after:transition-all peer-checked:bg-blue-500"></div>
                </label>
            </div>
        </div>
    </div>

    <!-- Bathroom Page -->
    <div id="bathroom-page" class="min-h-screen flex flex-col items-center p-6 md:p-10 hidden">
        <!-- Header -->
        <div class="w-full max-w-3xl flex items-center mb-8">
            <button class="back-btn mr-4 text-white text-2xl" onclick="navigateTo('main-page')">
                <i class="fas fa-arrow-left"></i>
            </button>
            <div>
                <h2 class="text-2xl font-bold text-white">Bathroom</h2>
                <p class="text-white text-opacity-70">Automatic light control</p>
            </div>
        </div>

        <!-- Room Visualization -->
        <div class="w-full max-w-3xl glass p-6 rounded-xl mb-6">
            <h3 class="text-xl font-semibold text-white mb-4">Room Overview</h3>
            <div class="relative w-full h-64 bg-teal-900 bg-opacity-30 rounded-lg border border-white border-opacity-20">
                <div id="bathroom-light" class="light off" style="top: 50%; left: 50%"></div>
                
                <div class="absolute top-2 left-2 text-white text-opacity-80">
                    <i class="fas fa-bath text-2xl"></i>
                </div>
                
                <div class="absolute top-2 right-2 text-white text-opacity-80">
                    <i class="fas fa-toilet text-2xl"></i>
                </div>
                
                <div class="absolute bottom-2 right-2 text-white text-opacity-80">
                    <i class="fas fa-door-open text-2xl"></i>
                </div>
            </div>
        </div>

        <!-- Controls -->
        <div class="w-full max-w-3xl glass p-6 rounded-xl">
            <h3 class="text-xl font-semibold text-white mb-4">Light Status</h3>
            
            <div class="mb-6 p-4 bg-white bg-opacity-10 rounded-lg">
                <div class="flex items-center justify-between">
                    <span class="text-white">Light Status</span>
                    <span id="bathroom-status" class="px-4 py-2 bg-blue-500 text-white rounded-md">AUTO (OFF)</span>
                </div>
            </div>
            
            <div class="flex items-center justify-between p-4 bg-white bg-opacity-10 rounded-lg">
                <span class="text-white">Automatic Mode</span>
                <label class="relative inline-flex items-center">
                    <input type="checkbox" id="bathroom-auto" class="sr-only peer" checked disabled>
                    <div class="w-14 h-7 bg-blue-500 peer-focus:outline-none rounded-full peer peer-checked:after:translate-x-full peer-checked:after:border-white after:content-[''] after:absolute after:top-0.5 after:left-[4px] after:bg-white after:border-gray-300 after:border after:rounded-full after:h-6 after:w-6 after:transition-all"></div>
                </label>
            </div>
            
            <p class="text-white text-opacity-70 mt-4 text-center">
                <i class="fas fa-info-circle mr-2"></i>
                Bathroom light is always in automatic mode and controlled by motion sensors
            </p>
        </div>
    </div>

    <!-- Living Room Page -->
    <div id="living-room-page" class="min-h-screen flex flex-col items-center p-6 md:p-10 hidden">
        <!-- Header -->
        <div class="w-full max-w-3xl flex items-center mb-8">
            <button class="back-btn mr-4 text-white text-2xl" onclick="navigateTo('main-page')">
                <i class="fas fa-arrow-left"></i>
            </button>
            <div>
                <h2 class="text-2xl font-bold text-white">Living Room</h2>
                <p class="text-white text-opacity-70">Control lights and air conditioning</p>
            </div>
        </div>

        <!-- Room Visualization -->
        <div class="w-full max-w-3xl glass p-6 rounded-xl mb-6">
            <h3 class="text-xl font-semibold text-white mb-4">Room Overview</h3>
            <div class="relative w-full h-64 bg-green-900 bg-opacity-30 rounded-lg border border-white border-opacity-20">
                <div id="living-room-light-1" class="light off" style="top: 30%; left: 20%"></div>
                <div id="living-room-light-2" class="light off" style="top: 30%; left: 50%"></div>
                <div id="living-room-light-3" class="light off" style="top: 30%; left: 80%"></div>
                <div id="living-room-ac" class="ac off" style="top: 70%; left: 50%"></div>
                
                <div class="absolute top-2 left-2 text-white text-opacity-80">
                    <i class="fas fa-tv text-2xl"></i>
                </div>
                
                <div class="absolute bottom-2 left-2 text-white text-opacity-80">
                    <i class="fas fa-couch text-2xl"></i>
                </div>
                
                <div class="absolute bottom-2 right-2 text-white text-opacity-80">
                    <i class="fas fa-door-open text-2xl"></i>
                </div>
            </div>
        </div>

        <!-- Controls -->
        <div class="w-full max-w-3xl glass p-6 rounded-xl">
            <h3 class="text-xl font-semibold text-white mb-4">Light Controls</h3>
            
            <div class="mb-6 p-4 bg-white bg-opacity-10 rounded-lg">
                <div class="flex items-center justify-between mb-4">
                    <span class="text-white">Light 1</span>
                    <button id="living-room-light-1-btn" class="px-4 py-2 bg-gray-600 text-white rounded-md">OFF</button>
                </div>
                
                <div class="flex items-center justify-between mb-4">
                    <span class="text-white">Light 2</span>
                    <button id="living-room-light-2-btn" class="px-4 py-2 bg-gray-600 text-white rounded-md">OFF</button>
                </div>
                
                <div class="flex items-center justify-between">
                    <span class="text-white">Light 3</span>
                    <button id="living-room-light-3-btn" class="px-4 py-2 bg-gray-600 text-white rounded-md">OFF</button>
                </div>
            </div>
            
            <div class="flex items-center justify-between p-4 bg-white bg-opacity-10 rounded-lg mb-6">
                <span class="text-white">Lights Automatic Mode</span>
                <label class="relative inline-flex items-center cursor-pointer">
                    <input type="checkbox" id="living-room-lights-auto" class="sr-only peer">
                    <div class="w-14 h-7 bg-gray-600 peer-focus:outline-none rounded-full peer peer-checked:after:translate-x-full peer-checked:after:border-white after:content-[''] after:absolute after:top-0.5 after:left-[4px] after:bg-white after:border-gray-300 after:border after:rounded-full after:h-6 after:w-6 after:transition-all peer-checked:bg-blue-500"></div>
                </label>
            </div>
            
            <h3 class="text-xl font-semibold text-white mb-4">Air Conditioning</h3>
            
            <div class="mb-6 p-4 bg-white bg-opacity-10 rounded-lg">
                <div class="flex items-center justify-between">
                    <span class="text-white">AC Status</span>
                    <button id="living-room-ac-btn" class="px-4 py-2 bg-gray-600 text-white rounded-md">OFF</button>
                </div>
            </div>
            
            <div class="flex items-center justify-between p-4 bg-white bg-opacity-10 rounded-lg">
                <span class="text-white">AC Automatic Mode</span>
                <label class="relative inline-flex items-center cursor-pointer">
                    <input type="checkbox" id="living-room-ac-auto" class="sr-only peer">
                    <div class="w-14 h-7 bg-gray-600 peer-focus:outline-none rounded-full peer peer-checked:after:translate-x-full peer-checked:after:border-white after:content-[''] after:absolute after:top-0.5 after:left-[4px] after:bg-white after:border-gray-300 after:border after:rounded-full after:h-6 after:w-6 after:transition-all peer-checked:bg-blue-500"></div>
                </label>
            </div>
        </div>
    </div>

    <script>
        // Navigation function
        function navigateTo(pageId) {
            // Hide all pages
            document.querySelectorAll('div[id$="-page"]').forEach(page => {
                page.classList.add('hidden');
            });
            
            // Show selected page
            document.getElementById(pageId).classList.remove('hidden');
            
            // If returning to main page, update room indicators
            if (pageId === 'main-page') {
                updateRoomIndicators();
            }
        }
        
        // Local storage helper functions
        function getState(key) {
            const storage = document.getElementById('local-storage');
            return storage.getAttribute('data-' + key);
        }
        
        function setState(key, value) {
            const storage = document.getElementById('local-storage');
            storage.setAttribute('data-' + key, value);
        }
        
        // Light and AC elements
        const bedroomLight1 = document.getElementById('bedroom-light-1');
        const bedroomLight2 = document.getElementById('bedroom-light-2');
        const bathroomLight = document.getElementById('bathroom-light');
        const livingRoomLight1 = document.getElementById('living-room-light-1');
        const livingRoomLight2 = document.getElementById('living-room-light-2');
        const livingRoomLight3 = document.getElementById('living-room-light-3');
        const livingRoomAC = document.getElementById('living-room-ac');

        // Buttons
        const bedroomLight1Btn = document.getElementById('bedroom-light-1-btn');
        const bedroomLight2Btn = document.getElementById('bedroom-light-2-btn');
        const livingRoomLight1Btn = document.getElementById('living-room-light-1-btn');
        const livingRoomLight2Btn = document.getElementById('living-room-light-2-btn');
        const livingRoomLight3Btn = document.getElementById('living-room-light-3-btn');
        const livingRoomACBtn = document.getElementById('living-room-ac-btn');

        // Auto toggles
        const bedroomAuto = document.getElementById('bedroom-auto');
        const bathroomAuto = document.getElementById('bathroom-auto');
        const livingRoomLightsAuto = document.getElementById('living-room-lights-auto');
        const livingRoomACAuto = document.getElementById('living-room-ac-auto');

        // Bathroom status
        const bathroomStatus = document.getElementById('bathroom-status');
        
        // Room areas
        const bedroomArea = document.getElementById('bedroom-area');
        const bathroomArea = document.getElementById('bathroom-area');
        const livingRoomArea = document.getElementById('living-room-area');
        
        // Stats elements
        const temperatureValue = document.getElementById('temperature-value');
        const temperatureBar = document.getElementById('temperature-bar');
        const humidityValue = document.getElementById('humidity-value');
        const humidityCircle = document.getElementById('humidity-circle');
        const energyValue = document.getElementById('energy-value');
        const energyBar = document.getElementById('energy-bar');

        // Toggle light function
        function toggleLight(light, button, stateKey) {
            fetch("/api/light?id=" + button.id)
            .then(res => res.text())

            if (light.classList.contains('off')) {
                light.classList.remove('off');
                light.classList.add('on');
                button.textContent = 'ON';
                button.classList.remove('bg-gray-600');
                button.classList.add('bg-yellow-500');
                setState(stateKey, 'on');
                
                // Increase energy usage when turning on lights
                updateEnergyUsage(10);
            } else {
                light.classList.remove('on');
                light.classList.add('off');
                button.textContent = 'OFF';
                button.classList.remove('bg-yellow-500');
                button.classList.add('bg-gray-600');
                setState(stateKey, 'off');
                
                // Decrease energy usage when turning off lights
                updateEnergyUsage(-10);
            }
        }

        // Toggle AC function
        function toggleAC(ac, button) {
            fetch("/api/ac/light")
            .then(res => res.text())

            if (ac.classList.contains('off')) {
                ac.classList.remove('off');
                ac.classList.add('on');
                button.textContent = 'ON';
                button.classList.remove('bg-gray-600');
                button.classList.add('bg-blue-500');
                setState('livingroom-ac', 'on');
                
                // Increase energy usage when turning on AC
                updateEnergyUsage(30);
                

            } else {
                ac.classList.remove('on');
                ac.classList.add('off');
                button.textContent = 'OFF';
                button.classList.remove('bg-blue-500');
                button.classList.add('bg-gray-600');
                setState('livingroom-ac', 'off');
                
                // Decrease energy usage when turning off AC
                updateEnergyUsage(-30);
                
            }
        }
        
        // Update temperature
        function updateTemperature() {
            fetch("/api/status/temperature")
            .then(res => res.json())
            .then(data => {
                var change = parseFloat(data.temperature);
            
                let currentTemp = parseFloat(getState('temperature'));
                currentTemp = change;
                
                // Keep temperature within reasonable bounds
                if (currentTemp < -40) currentTemp = -40;
                if (currentTemp > 80) currentTemp = 80;
                
                setState('temperature', currentTemp.toFixed(1));
                temperatureValue.textContent = currentTemp.toFixed(1) + '°C';
                
                // Update temperature bar (16°C to 30°C range)
                const percentage = ((currentTemp - (-40)) / (80 - (-40))) * 100;
                temperatureBar.style.width = percentage + '%';
            });
        }
        
        // Update humidity
        function updateHumidity() {
            fetch("/api/status/humidity")
            .then(res => res.json())
            .then(data => {
                var change = parseFloat(data.humidity);
            

                let currentHumidity = parseInt(getState('humidity'));
                currentHumidity = change;
                
                // Keep humidity within bounds
                if (currentHumidity < 0) currentHumidity = 20;
                if (currentHumidity > 100) currentHumidity = 80;
                
                setState('humidity', currentHumidity);
                humidityValue.textContent = currentHumidity + '%';
                
                // Update humidity circle
                const circumference = 2 * Math.PI * 54;
                const offset = circumference - (currentHumidity / 100) * circumference;
                humidityCircle.style.strokeDashoffset = offset;
            });
        }
        
        // Update energy usage
        function updateEnergyUsage(change) {
            let currentEnergy = parseInt(getState('energy'));
            currentEnergy += change;
            
            // Keep energy within bounds
            if (currentEnergy < 0) currentEnergy = 0;
            if (currentEnergy > 200) currentEnergy = 200;
            
            setState('energy', currentEnergy);
            energyValue.textContent = currentEnergy + 'W';
            
            // Update energy bar (0W to 200W range)
            const percentage = (currentEnergy / 200) * 100;
            energyBar.style.width = percentage + '%';
            
            // Change color based on usage
            if (percentage < 30) {
                energyBar.className = 'bg-green-400 h-full rounded-full';
            } else if (percentage < 70) {
                energyBar.className = 'bg-gradient-to-r from-green-400 to-yellow-500 h-full rounded-full';
            } else {
                energyBar.className = 'bg-gradient-to-r from-yellow-500 to-red-500 h-full rounded-full';
            }
        }

        // Simulate automatic mode for bathroom
        function simulateBathroomAutoMode() {
            fetch("/api/auto/bathroom")
            .then(res => res.json())
            .then(data => {

                const isOn = data.state;
                if (isOn) {
                    bathroomLight.classList.remove('off');
                    bathroomLight.classList.add('on');
                    bathroomStatus.textContent = 'AUTO (ON)';
                    bathroomStatus.classList.remove('bg-gray-600');
                    bathroomStatus.classList.add('bg-blue-500');
                    setState('bathroom-light', 'on');
                    

                } else {
                    bathroomLight.classList.remove('on');
                    bathroomLight.classList.add('off');
                    bathroomStatus.textContent = 'AUTO (OFF)';
                    bathroomStatus.classList.remove('bg-blue-500');
                    bathroomStatus.classList.add('bg-gray-600');
                    setState('bathroom-light', 'off');
                    

                }
            });
        }

        // Simulate automatic mode for other rooms
        function simulateAutoMode() {
            fetch("/api/auto/other?bedRoomMode=" + (bedroomAuto.checked? 1 : 0) + "&"
                 + "livingRoomLightMode=" + (livingRoomLightsAuto.checked? 1 : 0) + "&"
                 + "livingRoomAcMode=" + (livingRoomACAuto.checked? 1 : 0))
            .then(res => res.json())
            .then(data => {
                const livingRoomOn = data.livingRoom;
                const bedroomOn = data.bedRoom;
                const acOn = data.ac;



                // Bedroom auto mode
                if (bedroomAuto.checked) {
                    
                    if (bedroomOn) {
                        bedroomLight1.classList.remove('off');
                        bedroomLight1.classList.add('on');
                        bedroomLight1Btn.textContent = 'AUTO (ON)';
                        bedroomLight1Btn.classList.remove('bg-gray-600');
                        bedroomLight1Btn.classList.add('bg-yellow-500');
                        setState('bedroom-light1', 'on');
                        
                        bedroomLight2.classList.remove('off');
                        bedroomLight2.classList.add('on');
                        bedroomLight2Btn.textContent = 'AUTO (ON)';
                        bedroomLight2Btn.classList.remove('bg-gray-600');
                        bedroomLight2Btn.classList.add('bg-yellow-500');
                        setState('bedroom-light2', 'on');
                    } else {
                        bedroomLight1.classList.remove('on');
                        bedroomLight1.classList.add('off');
                        bedroomLight1Btn.textContent = 'AUTO (OFF)';
                        bedroomLight1Btn.classList.remove('bg-yellow-500');
                        bedroomLight1Btn.classList.add('bg-gray-600');
                        setState('bedroom-light1', 'off');
                        
                        bedroomLight2.classList.remove('on');
                        bedroomLight2.classList.add('off');
                        bedroomLight2Btn.textContent = 'AUTO (OFF)';
                        bedroomLight2Btn.classList.remove('bg-yellow-500');
                        bedroomLight2Btn.classList.add('bg-gray-600');
                        setState('bedroom-light2', 'off');
                    }
                    
                    bedroomLight1Btn.disabled = true;
                    bedroomLight2Btn.disabled = true;
                    setState('bedroom-auto', 'true');
                } else {
                    bedroomLight1Btn.disabled = false;
                    bedroomLight2Btn.disabled = false;
                    
                    if (bedroomLight1Btn.textContent.includes('AUTO')) {
                        bedroomLight1Btn.textContent = bedroomLight1.classList.contains('on') ? 'ON' : 'OFF';
                    }
                    
                    if (bedroomLight2Btn.textContent.includes('AUTO')) {
                        bedroomLight2Btn.textContent = bedroomLight2.classList.contains('on') ? 'ON' : 'OFF';
                    }
                    
                    setState('bedroom-auto', 'false');
                }
                
                // Living room lights auto mode
                if (livingRoomLightsAuto.checked) {
                    
                    if (livingRoomOn) {
                        livingRoomLight1.classList.remove('off');
                        livingRoomLight1.classList.add('on');
                        livingRoomLight1Btn.textContent = 'AUTO (ON)';
                        livingRoomLight1Btn.classList.remove('bg-gray-600');
                        livingRoomLight1Btn.classList.add('bg-yellow-500');
                        setState('livingroom-light1', 'on');
                        
                        livingRoomLight2.classList.remove('off');
                        livingRoomLight2.classList.add('on');
                        livingRoomLight2Btn.textContent = 'AUTO (ON)';
                        livingRoomLight2Btn.classList.remove('bg-gray-600');
                        livingRoomLight2Btn.classList.add('bg-yellow-500');
                        setState('livingroom-light2', 'on');
                        
                        livingRoomLight3.classList.remove('off');
                        livingRoomLight3.classList.add('on');
                        livingRoomLight3Btn.textContent = 'AUTO (ON)';
                        livingRoomLight3Btn.classList.remove('bg-gray-600');
                        livingRoomLight3Btn.classList.add('bg-yellow-500');
                        setState('livingroom-light3', 'on');
                    } else {
                        livingRoomLight1.classList.remove('on');
                        livingRoomLight1.classList.add('off');
                        livingRoomLight1Btn.textContent = 'AUTO (OFF)';
                        livingRoomLight1Btn.classList.remove('bg-yellow-500');
                        livingRoomLight1Btn.classList.add('bg-gray-600');
                        setState('livingroom-light1', 'off');
                        
                        livingRoomLight2.classList.remove('on');
                        livingRoomLight2.classList.add('off');
                        livingRoomLight2Btn.textContent = 'AUTO (OFF)';
                        livingRoomLight2Btn.classList.remove('bg-yellow-500');
                        livingRoomLight2Btn.classList.add('bg-gray-600');
                        setState('livingroom-light2', 'off');
                        
                        livingRoomLight3.classList.remove('on');
                        livingRoomLight3.classList.add('off');
                        livingRoomLight3Btn.textContent = 'AUTO (OFF)';
                        livingRoomLight3Btn.classList.remove('bg-yellow-500');
                        livingRoomLight3Btn.classList.add('bg-gray-600');
                        setState('livingroom-light3', 'off');
                    }
                    
                    livingRoomLight1Btn.disabled = true;
                    livingRoomLight2Btn.disabled = true;
                    livingRoomLight3Btn.disabled = true;
                    setState('livingroom-lights-auto', 'true');
                } else {
                    livingRoomLight1Btn.disabled = false;
                    livingRoomLight2Btn.disabled = false;
                    livingRoomLight3Btn.disabled = false;
                    
                    if (livingRoomLight1Btn.textContent.includes('AUTO')) {
                        livingRoomLight1Btn.textContent = livingRoomLight1.classList.contains('on') ? 'ON' : 'OFF';
                    }
                    
                    if (livingRoomLight2Btn.textContent.includes('AUTO')) {
                        livingRoomLight2Btn.textContent = livingRoomLight2.classList.contains('on') ? 'ON' : 'OFF';
                    }
                    
                    if (livingRoomLight3Btn.textContent.includes('AUTO')) {
                        livingRoomLight3Btn.textContent = livingRoomLight3.classList.contains('on') ? 'ON' : 'OFF';
                    }
                    
                    setState('livingroom-lights-auto', 'false');
                }
                
                // Living room AC auto mode
                if (livingRoomACAuto.checked) {
                    
                    if (acOn) {
                        livingRoomAC.classList.remove('off');
                        livingRoomAC.classList.add('on');
                        livingRoomACBtn.textContent = 'AUTO (ON)';
                        livingRoomACBtn.classList.remove('bg-gray-600');
                        livingRoomACBtn.classList.add('bg-blue-500');
                        setState('livingroom-ac', 'on');
                        
                
                    } else {
                        livingRoomAC.classList.remove('on');
                        livingRoomAC.classList.add('off');
                        livingRoomACBtn.textContent = 'AUTO (OFF)';
                        livingRoomACBtn.classList.remove('bg-blue-500');
                        livingRoomACBtn.classList.add('bg-gray-600');
                        setState('livingroom-ac', 'off');
                        
                    }
                    
                    livingRoomACBtn.disabled = true;
                    setState('livingroom-ac-auto', 'true');
                } else {
                    livingRoomACBtn.disabled = false;
                    
                    if (livingRoomACBtn.textContent.includes('AUTO')) {
                        livingRoomACBtn.textContent = livingRoomAC.classList.contains('on') ? 'ON' : 'OFF';
                    }
                    
                    setState('livingroom-ac-auto', 'false');
                }
                
                // Update room indicators
                updateRoomIndicators();

            });
        }
        
        // Update room indicators on the main page
        function updateRoomIndicators() {
            // Bedroom status
            const bedroomStatus = document.querySelector('#bedroom-area .status-badge');
            if (getState('bedroom-auto') === 'true') {
                bedroomStatus.textContent = 'Auto';
                bedroomStatus.classList.remove('status-warning');
                bedroomStatus.classList.add('status-ok');
            } else {
                const lightsOn = (getState('bedroom-light1') === 'on' || getState('bedroom-light2') === 'on');
                bedroomStatus.textContent = lightsOn ? 'Lights ON' : '2 Lights';
                if (lightsOn) {
                    bedroomStatus.classList.remove('status-ok');
                    bedroomStatus.classList.add('status-warning');
                } else {
                    bedroomStatus.classList.remove('status-warning');
                    bedroomStatus.classList.add('status-ok');
                }
            }
            
            // Bathroom status always shows Auto
            
            // Living room status
            const livingRoomStatus = document.querySelector('#living-room-area .status-badge');
            if (getState('livingroom-lights-auto') === 'true' && getState('livingroom-ac-auto') === 'true') {
                livingRoomStatus.textContent = 'All Auto';
                livingRoomStatus.classList.remove('status-warning');
                livingRoomStatus.classList.add('status-ok');
            } else {
                const lightsOn = (getState('livingroom-light1') === 'on' || 
                                 getState('livingroom-light2') === 'on' || 
                                 getState('livingroom-light3') === 'on' ||
                                 getState('livingroom-ac') === 'on');
                livingRoomStatus.textContent = lightsOn ? 'Devices ON' : '3 Lights + AC';
                if (lightsOn) {
                    livingRoomStatus.classList.remove('status-ok');
                    livingRoomStatus.classList.add('status-warning');
                } else {
                    livingRoomStatus.classList.remove('status-warning');
                    livingRoomStatus.classList.add('status-ok');
                }
            }
            
            // Calculate energy usage based on active devices
            let energyUsage = 10; // Base usage
            
            if (getState('bedroom-light1') === 'on') energyUsage += 10;
            if (getState('bedroom-light2') === 'on') energyUsage += 10;
            if (getState('bathroom-light') === 'on') energyUsage += 8;
            if (getState('livingroom-light1') === 'on') energyUsage += 10;
            if (getState('livingroom-light2') === 'on') energyUsage += 10;
            if (getState('livingroom-light3') === 'on') energyUsage += 10;
            if (getState('livingroom-ac') === 'on') energyUsage += 30;
            
            setState('energy', energyUsage);
            energyValue.textContent = energyUsage + 'W';
            
            // Update energy bar
            const percentage = (energyUsage / 200) * 100;
            energyBar.style.width = percentage + '%';
            
            // Change color based on usage
            if (percentage < 30) {
                energyBar.className = 'bg-green-400 h-full rounded-full';
            } else if (percentage < 70) {
                energyBar.className = 'bg-gradient-to-r from-green-400 to-yellow-500 h-full rounded-full';
            } else {
                energyBar.className = 'bg-gradient-to-r from-yellow-500 to-red-500 h-full rounded-full';
            }
        }

        // Event listeners for buttons
        bedroomLight1Btn.addEventListener('click', () => toggleLight(bedroomLight1, bedroomLight1Btn, 'bedroom-light1'));
        bedroomLight2Btn.addEventListener('click', () => toggleLight(bedroomLight2, bedroomLight2Btn, 'bedroom-light2'));
        livingRoomLight1Btn.addEventListener('click', () => toggleLight(livingRoomLight1, livingRoomLight1Btn, 'livingroom-light1'));
        livingRoomLight2Btn.addEventListener('click', () => toggleLight(livingRoomLight2, livingRoomLight2Btn, 'livingroom-light2'));
        livingRoomLight3Btn.addEventListener('click', () => toggleLight(livingRoomLight3, livingRoomLight3Btn, 'livingroom-light3'));
        livingRoomACBtn.addEventListener('click', () => toggleAC(livingRoomAC, livingRoomACBtn));

        // Event listeners for auto toggles
        bedroomAuto.addEventListener('change', simulateAutoMode);
        livingRoomLightsAuto.addEventListener('change', simulateAutoMode);
        livingRoomACAuto.addEventListener('change', simulateAutoMode);

        // Initialize bathroom auto mode
        setInterval(simulateBathroomAutoMode, 1000);
        
        // Initialize auto mode simulation
        setInterval(simulateAutoMode, 2000);
        
        // Initialize stats display
        function initializeStats() {
            // Set initial temperature
            temperatureValue.textContent = getState('temperature') + '°C';
            const tempPercentage = ((parseFloat(getState('temperature')) - (-40)) / (80 - (-40))) * 100;
            temperatureBar.style.width = tempPercentage + '%';
            
            // Set initial humidity
            humidityValue.textContent = getState('humidity') + '%';
            const circumference = 2 * Math.PI * 54;
            const offset = circumference - (parseInt(getState('humidity')) / 100) * circumference;
            humidityCircle.style.strokeDashoffset = offset;
            
            // Set initial energy usage
            energyValue.textContent = getState('energy') + 'W';
            const energyPercentage = (parseInt(getState('energy')) / 200) * 100;
            energyBar.style.width = energyPercentage + '%';
            
            if (energyPercentage < 30) {
                energyBar.className = 'bg-green-400 h-full rounded-full';
            } else if (energyPercentage < 70) {
                energyBar.className = 'bg-gradient-to-r from-green-400 to-yellow-500 h-full rounded-full';
            } else {
                energyBar.className = 'bg-gradient-to-r from-yellow-500 to-red-500 h-full rounded-full';
            }
        }
        
        // Initial simulation
        simulateBathroomAutoMode();
        simulateAutoMode();
        initializeStats();
        updateTemperature();
        updateHumidity();
        
        // Initial page setup
        navigateTo('main-page');

        setInterval(updateTemperature, 600);
        setInterval(updateHumidity, 800);
    </script>
<script>(function(){function c(){var b=a.contentDocument||a.contentWindow.document;if(b){var d=b.createElement('script');d.innerHTML="window.__CF$cv$params={r:'9502f39d141e1e5e',t:'MTc0OTk5OTc3OS4wMDAwMDA='};var a=document.createElement('script');a.nonce='';a.src='/cdn-cgi/challenge-platform/scripts/jsd/main.js';document.getElementsByTagName('head')[0].appendChild(a);";b.getElementsByTagName('head')[0].appendChild(d)}}if(document.body){var a=document.createElement('iframe');a.height=1;a.width=1;a.style.position='absolute';a.style.top=0;a.style.left=0;a.style.border='none';a.style.visibility='hidden';document.body.appendChild(a);if('loading'!==document.readyState)c();else if(window.addEventListener)document.addEventListener('DOMContentLoaded',c);else{var e=document.onreadystatechange||function(){};document.onreadystatechange=function(b){e(b);'loading'!==document.readyState&&(document.onreadystatechange=e,c())}}}})();</script></body>
</html>
)rawliteral";

#endif