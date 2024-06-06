// Разработанная библиотека-шаблон
#include <SmartThing.h>
// Библиотека для обновления по беспроводной сети
#include <ArduinoOTA.h>
// Библиотека для работы с метеосенсором
#include <GyverBME280.h>

void addSensors();

// Метесенсор
GyverBME280 bme;

void test() {

  SmartThing.addAnalogSensor("temp", 1);
  SmartThing.addDigitalSensor("button", 2);
  SmartThing.addSensor("pressure", []() {
    return bme.readPressure() * 0.00750062;
  });

  SmartThing.addDeviceState("wifi", []() {
    return SmartThing.wifiConnected() ? 
      "connected" : "disconnected";
  });
}

// Настройка перед запуском
void setup() {
  // Добавление сенсоров
  addSensors();

  // Инициализация бибилотеки с указанием типа устройства
  if (SmartThing.init("meteo_station")) {
    LOGGER.info("main", "SmartThing successfully initialized");
  } else {
    LOGGER.error("main", "Failed to init SmartThing!");
  }
  
  // Проверка наличия подключения к беспроводной сети
  // и запуск запуск библиотеки для получения
  // обновлений по беспроводной сети
  if (SmartThing.wifiConnected()) {
    ArduinoOTA.begin();
  }

  // Запуск метесенсора
  if (!bme.begin(0x76)) {
    LOGGER.error("main", "Bme initialization error!");
  }
  LOGGER.info("main", "Setup finished");
}

// Метод, который работает бесконечно в цикле
void loop() {
  if (SmartThing.wifiConnected()) {
    // Получение входящих подключений для обновления прошивки
    ArduinoOTA.handle();
  }
  delay(500);
}

void addSensors() {
  // Добавление различных сенсоров при помощи разработанной библиотеки
  // Показатель температуры
  SmartThing.addSensor("temperature", []() {
    return bme.readTemperature();
  });
  // Показатель влажности
  SmartThing.addSensor("humidity", []() {
    return bme.readHumidity();
  });
  // Показатель давления
  SmartThing.addSensor("pressure", []() {
    return bme.readPressure() * 0.00750062;
  });
}
