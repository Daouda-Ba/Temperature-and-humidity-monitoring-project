from django.contrib import admin
from .models import Mesure

@admin.register(Mesure)
class MesureAdmin(admin.ModelAdmin):
    list_display = ('timestamp', 'temperature', 'humidite')
    list_filter = ('timestamp',)
    ordering = ('-timestamp',)

