const int trigPin1 = 2; // Pin de trigger del primer sensor
const int echoPin1 = 3; // Pin de echo del primer sensor
const int trigPin2 = 4; // Pin de trigger del segundo sensor
const int echoPin2 = 5; // Pin de echo del segundo sensor
const float velocidadSonido = 0.000343; // Velocidad del sonido en m/μs

bool objetoCayendo = false;
unsigned long tiempoInicio = 0;
unsigned long tiempoFin = 0;
float distanciaSensor1 = 0;
float distanciaSensor2 = 0;
float distanciaEntreSensores = 0; // En metros

void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

void loop() {
  verificarPrimerSensor();
  verificarSegundoSensor();
}

void verificarPrimerSensor() {
  if (!objetoCayendo && detectarObjeto(trigPin1, echoPin1)) {
    objetoCayendo = true;
    tiempoInicio = micros(); // Tiempo en microsegundos
    // Aquí establecemos la distancia del primer sensor a cero
    distanciaSensor1 = 0; // Distancia del primer sensor inicializada en cero metros
  }
}

void verificarSegundoSensor() {
  if (objetoCayendo && detectarObjeto(trigPin2, echoPin2)) {
    tiempoFin = micros(); // Tiempo en microsegundos
    objetoCayendo = false;
    distanciaSensor2 = medirDistancia(trigPin2, echoPin2); // Medir distancia del segundo sensor

    distanciaEntreSensores = calcularDistanciaEntreSensores(distanciaSensor1, distanciaSensor2);
    
    Serial.print("Distancia entre sensores medida automáticamente: ");
    Serial.print(distanciaEntreSensores, 6); // Mayor precisión con 6 decimales
    Serial.println(" metros");

    calcularDistancia();
    reiniciarValores();
  }
}

bool detectarObjeto(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  float distancia = (duration * velocidadSonido) / 2.0;
  return (distancia > 0 && distancia < 0.08); // Se considera que hay un objeto si la distancia está en un rango entre 0 y 10 metros
}

float medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  float distancia = (duration * velocidadSonido) / 2.0; // Distancia en metros
  return distancia; // Ya en metros
}

float calcularDistanciaEntreSensores(float distancia1, float distancia2) {
  // Suponiendo que los sensores están a una altura fija y horizontalmente alineados
  return abs(distancia2 - distancia1); // Distancia en metros
}

void calcularDistancia() {
  float tiempoTotal = (tiempoFin - tiempoInicio) / 1000000.0; // Convertir de microsegundos a segundos
  float tiempoCuadrado = tiempoTotal * tiempoTotal; // Calcular el cuadrado del tiempo de caída
  float gravedadExperimental = (2 * distanciaEntreSensores) / tiempoCuadrado; // Calcular la gravedad experimental
  float distanciaCalculada = 0.5 * gravedadExperimental * tiempoCuadrado; // Calcular la distancia de caída
  float velocidadFinal = gravedadExperimental * tiempoTotal; // Calcular la velocidad final

  // Imprimir resultados con mayor precisión
  Serial.print("Tiempo total de caída: ");
  Serial.print(tiempoTotal, 6); // Mayor precisión con 6 decimales
  Serial.print("Tiempo inicial: ");
  Serial.print(tiempoInicio);
  Serial.print("Tiempo final: ");
  Serial.print(tiempoFin);

  Serial.println(" segundos");
  Serial.print("Distancia entre sensores: ");
  Serial.print(distanciaEntreSensores, 6); // Mayor precisión con 6 decimales
  Serial.println(" metros");
  Serial.print("Distancia calculada de caída: ");
  Serial.print(distanciaCalculada, 6); // Mayor precisión con 6 decimales
  Serial.println(" metros");
  Serial.print("Gravedad experimental: ");
  Serial.print(gravedadExperimental, 6); // Mayor precisión con 6 decimales
  Serial.println(" m/s^2");
  Serial.print("Velocidad final: ");
  Serial.print(velocidadFinal, 6); // Mayor precisión con 6 decimales
  Serial.println(" m/s");
}

void reiniciarValores() {
  objetoCayendo = false;
  tiempoInicio = 0;
  tiempoFin = 0;
  distanciaSensor1 = 0;
  distanciaSensor2 = 0;
  distanciaEntreSensores = 0; // También es importante reiniciar la distancia entre sensores
}
