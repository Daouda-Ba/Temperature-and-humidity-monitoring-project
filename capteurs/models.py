from django.db import models

class Mesure(models.Model):
    temperature = models.FloatField()
    humidite = models.FloatField()
    timestamp = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"{self.timestamp} - T: {self.temperature}°C, H: {self.humidite}%"

