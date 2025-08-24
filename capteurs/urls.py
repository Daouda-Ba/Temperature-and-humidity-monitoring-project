from django.urls import path
from .views import recevoir_donnees, dashboard

urlpatterns = [
    path('api/recevoir/', recevoir_donnees, name='recevoir_donnees'),
    path('dashboard/', dashboard, name='dashboard'),
]