

#define DUE_TIME_OUT    7 // период ожидания сигнала дуе (сек)
                        
#define RELAY_ON        0 // реле включается нулем
#define RELAY_OFF       1 // а выключается единицей

#define LED_PIN         13
#define STOP_WATCH_PIN  9 // 0 на этом входе останавливает наблюдение
#define WATCH_PIN_OUT   8 // идет на 27 пин дуе
#define WATCH_PIN_IN    7 // идет на 25 пин дуе
#define RELAY_PIN       6 // дуе подключена к нормально-замкнутому контакту реле
                          // чтобы ее перезагрузить, реле нужно включить

void setup(){
  Serial.begin(9600);
  Serial.print("Start\n");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);

  pinMode(WATCH_PIN_IN, INPUT);
  digitalWrite(WATCH_PIN_IN, HIGH);
  
  pinMode(STOP_WATCH_PIN, INPUT);
  digitalWrite(STOP_WATCH_PIN, HIGH);
    
  pinMode(WATCH_PIN_OUT, INPUT);    // пин должен быть в свободном состоянии для возможности ручной перезагрузки
  digitalWrite(WATCH_PIN_OUT, LOW); // на всякий случай выключаем подтягивающий резистор
}

long t = 0;
long count = DUE_TIME_OUT;
bool WatchIn, WatchOut;

void loop(){
  
  if (digitalRead(WATCH_PIN_IN) == WatchIn){              // ищем сигналы по 7 пину от дуе
    WatchIn = !WatchIn;
    Serial.println("Pin 7 = " + (String)digitalRead(WATCH_PIN_IN));
    count = DUE_TIME_OUT;                                 // если нашли - перезапускаем счетчик
  }
  
  if ((millis() - t) > 1000){                               // заходим сюда раз в секунту
    t = millis();

    if (digitalRead(STOP_WATCH_PIN)){
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));           // моргаем светодиодом
      
      
      count--;
      Serial.println("  count = " + (String)count);
      if (count <= 0){                                        // если не нашли - перезагружаем дуе
        digitalWrite(RELAY_PIN, RELAY_ON);
        digitalWrite(LED_PIN, HIGH);                          // сигнал светодиодом
        delay(500);
        digitalWrite(LED_PIN, LOW);                           // сигнал светодиодом
        digitalWrite(RELAY_PIN, RELAY_OFF);
        
        pinMode(WATCH_PIN_OUT, OUTPUT);                       // переводим пин в режим выхода
        digitalWrite(WATCH_PIN_OUT, LOW);                     // нажимаем ресет, а то бывает не запускается
        delay(250);
        digitalWrite(WATCH_PIN_OUT, HIGH);                    // отпускаем ресет
        pinMode(WATCH_PIN_OUT, INPUT);                        // переводим пин в режим входа
        digitalWrite(WATCH_PIN_OUT, LOW);                     // отпускаем подтягивающий резистор
        count = DUE_TIME_OUT;                                 // и перезагружаем счетчик
      } 
    }
  }
  
}
