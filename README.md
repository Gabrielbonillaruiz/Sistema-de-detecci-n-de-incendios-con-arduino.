# Sistema-de-detección-de-incendios-con-arduino.

El sistema de detección de incendios se ha desarrollado en Arduino. Se utiliza un dispositivo LM35 para la toma de la temperatura ambiental, conectando la patilla 2 en la entrada analógica A5 de la placa y las patillas 1 y 3 a VCC y GND respectivamente. 

El aviso de incendio se generará en los siguientes casos:


Incremento drástico de la temperatura ambiental en al menos 10º C.
Temperatura ambiental por encima de 60º C.

La frecuencia de la toma de muestras es ajustable mediante la variable de programa “FrecuenciaMuestras”.

Para la comunicación con las autoridades correspondientes, se utiliza un módulo GSM con el que se manda un mensaje de texto con las coordenadas geográficas exactas del dispositivo detector.
Se ha añadido al sistema una pantalla TFT para la observación en tiempo real del correcto funcionamiento de la aplicación, ofreciendo un histórico de resultados imprimidos por pantalla.
