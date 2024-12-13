// Разработанная библиотека-шаблон
#include <SmartThing.h>
// Библиотека для обновления по беспроводной сети
#include <ArduinoOTA.h>
// Библиотека для работы с датчиком давления
#include <Adafruit_BMP280.h>
// Библиотека для работы с датчиком температуры и влажности
#include <AHT10.h>

void addSensors();

// Датчик давления
Adafruit_BMP280 bmp;
// Датчик температуры и влажности
AHT10 aht(AHT10_ADDRESS_0X38, AHT20_SENSOR);

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

  // Запуск датчиков
  if (bmp.begin()) {
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    LOGGER.info("main", "Bmp sensor initialization finished!");
  } else {
    LOGGER.error("main", "Bmp sensor initialization error!");
  }

  if (aht.begin()) {
    LOGGER.info("main", "Aht sensor initialization finished!");
  } else {
    LOGGER.error("main", "Aht sensor initialization error!");
  }

  LOGGER.info("main", "Setup finished");
}

// Метод, который работает бесконечно в цикле
void loop() {
  if (SmartThing.wifiConnected()) {
    // Получение входящих подключений для обновления прошивки
    ArduinoOTA.handle();
  }
  delay(250);
}

void addSensors() {
  // Добавление различных сенсоров при помощи разработанной библиотеки
  // // Показатель температуры
  SensorsManager.addSensor("temperature", []() {
    return aht.readTemperature(AHT10_FORCE_READ_DATA);
  });
  // // Показатель влажности
  SensorsManager.addSensor("humidity", []() {
    return aht.readHumidity(AHT10_FORCE_READ_DATA);
  });
  // Показатель давления
  SensorsManager.addSensor("pressure", []() {
    return bmp.readPressure() * 0.00750062;
  });
}
