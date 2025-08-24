# 🌡Système IoT de Supervision Environnementale

Un système IoT complet pour la surveillance en temps réel de la température et de l'humidité avec ESP8266, capteur DHT11 et dashboard Django.

## Table des matières
- [Aperçu](#aperçu)
- [Objectifs](#objectifs)
- [Architecture du système](#architecture-du-système)
- [Composants matériels](#composants-matériels)
- [Technologies utilisées](#technologies-utilisées)
- [Fonctionnalités](#fonctionnalités)
- [Installation](#installation)
- [Configuration](#configuration)
- [Utilisation](#utilisation)
- [API Endpoints](#api-endpoints)
- [Améliorations possibles](#améliorations-possibles)
- [Dépannage](#dépannage)
- [Contribution](#contribution)
- [Licence](#licence)

## Aperçu

Ce projet implémente un système IoT de supervision environnementale permettant la mesure, la transmission et la visualisation en temps réel de données de température et d'humidité. Le système combine hardware (ESP8266 + DHT11) et software (Django + Chart.js) pour une solution complète de monitoring.

## Objectifs

Le système vise à :
- **Mesurer** en temps réel la température et l'humidité via un capteur DHT11
- **Transmettre** les données sans fil via Wi-Fi grâce au microcontrôleur ESP8266
- **Stocker** les données dans une base de données SQLite via un serveur Django
- **Afficher** les données sur une interface web interactive avec des graphiques dynamiques
- **Alerter** par email en cas de dépassement de seuils critiques

## Architecture du Système

```
[DHT11] ➜ [ESP8266] ➜ [Wi-Fi] ➜ [Django API] ➜ [SQLite DB] ➜ [Dashboard Web]
                                        ⬇️
                                   [Email Alerts]
```

Le système suit une architecture IoT classique :
1. **Couche Capteurs** : DHT11 pour mesures environnementales
2. **Couche Communication** : ESP8266 avec Wi-Fi
3. **Couche Application** : API Django pour traitement des données
4. **Couche Présentation** : Dashboard web interactif
5. **Couche Notification** : Système d'alertes par email

## Composants Matériels

| Composant | Description | Quantité |
|-----------|-------------|----------|
| **ESP8266 NodeMCU** | Microcontrôleur avec module Wi-Fi intégré | 1 |
| **Capteur DHT11** | Capteur numérique température/humidité | 1 |
| **LED verte** | Indicateur visuel état normal | 1 |
| **LED rouge** | Indicateur visuel état d'alerte | 1 |
| **Résistances 220Ω** | Protection des LEDs | 2 |
| **Breadboard** | Plaque de prototypage | 1 |
| **Fils de connexion** | Câblage du circuit | Selon besoin |

### Schéma de câblage

```
ESP8266 NodeMCU:
├── Pin D4 ➜ DHT11 (Data)
├── Pin D1 ➜ LED Verte (via résistance 220Ω)
├── Pin D2 ➜ LED Rouge (via résistance 220Ω)
├── 3.3V ➜ DHT11 (VCC)
└── GND ➜ DHT11, LEDs (GND)
```

## Technologies Utilisées

### Hardware
- **ESP8266** : Microcontrôleur Wi-Fi
- **DHT11** : Capteur température/humidité

### Software
- **Arduino IDE** : Développement embarqué
- **Python 3.8+** : Langage backend
- **Django 4.0+** : Framework web et API REST
- **SQLite** : Base de données légère
- **Chart.js** : Visualisation de données
- **HTML/CSS/JavaScript** : Interface utilisateur
- **Brevo API** : Service d'envoi d'emails

### Outils de développement
- **Postman** : Test des APIs
- **Git** : Contrôle de version
- **VS Code** : Éditeur de code

## Fonctionnalités

### Monitoring en temps réel
- Mesure automatique toutes les 15 secondes
- Affichage instantané des valeurs actuelles
- Graphiques historiques interactifs

### Interface Web
- Dashboard responsive et moderne
- Tableau des mesures récentes
- Graphiques dynamiques (Chart.js)
- Indicateurs visuels d'état

### Système d'alertes
- Notification par email si température > 40°C
- LEDs d'indication d'état sur le dispositif
- Logs d'événements dans l'interface


## Installation

### 1. Prérequis
```bash
# Python 3.8 ou supérieur
python --version

# Arduino IDE installé
# ESP8266 board package configuré
```

### 2. Cloner le repository
```bash
git clone https://github.com/Daouda-Ba/Temperature-and-humidity-monitoring.git
cd Temperature-and-humidity-monitoring
```

### 3. Configuration de l'environnement Python
```bash
# Créer un environnement virtuel
python -m venv venv

# Activer l'environnement
source venv/bin/activate  # Linux/Mac
venv\Scripts\activate     # Windows


### 4. Configuration de la base de données
```bash
# Migrations Django
python manage.py makemigrations
python manage.py migrate

# Créer un superuser (optionnel)
python manage.py createsuperuser
```

## Configuration

### 1. Configuration ESP8266

Créez un fichier `config.h` dans le dossier Arduino :

```cpp
// config.h
#define WIFI_SSID "votre_wifi_ssid"
#define WIFI_PASSWORD "votre_wifi_password"
#define SERVER_URL "http://votre-serveur-ip:8000/api/recevoir/"
#define DHT_PIN 2  // Pin D4
#define LED_GREEN_PIN 5  // Pin D1
#define LED_RED_PIN 4    // Pin D2
```

### 2. Configuration Django

Créez un fichier `.env` dans le dossier Django :

```bash
# .env
SECRET_KEY=votre_secret_key_django
DEBUG=True
ALLOWED_HOSTS=localhost,127.0.0.1,votre-ip

# Configuration email (Brevo)
EMAIL_HOST=smtp-relay.brevo.com
EMAIL_PORT=587
EMAIL_USE_TLS=True
EMAIL_HOST_USER=votre_email@example.com
EMAIL_HOST_PASSWORD=votre_api_key_brevo

# Seuils d'alerte
TEMP_THRESHOLD=40
HUMIDITY_THRESHOLD=80
```

## Utilisation

### 1. Démarrer le serveur Django
```bash
python manage.py runserver 0.0.0.0:8000
```

### 2. Programmer l'ESP8266
1. Ouvrir Arduino IDE
2. Charger le sketch `esp8266_sensor.ino`
3. Configurer les paramètres Wi-Fi
4. Téléverser sur l'ESP8266

### 3. Accéder au dashboard
Ouvrir votre navigateur à : `http://localhost:8000`

### 4. Monitoring
- Les données s'affichent automatiquement
- Les graphiques se mettent à jour en temps réel
- Les alertes sont envoyées si seuils dépassés

## API Endpoints

### POST /api/recevoir/
**Description** : Réceptionne les données des capteurs

**Format JSON** :
```json
{
    "temperature": 25.5,
    "humidity": 60.2,
    "timestamp": "2025-06-23T10:30:00Z"
}
```

**Réponse** :
```json
{
    "status": "success",
    "message": "Données reçues et enregistrées"
}
```

### GET /api/data/
**Description** : Récupère les données historiques

**Paramètres** :
- `limit` : Nombre de mesures (défaut: 50)
- `hours` : Données des X dernières heures


## Améliorations Possibles


### Moyen terme
- [ ] Migration vers protocole MQTT
- [ ] Capteurs supplémentaires (CO2, lumière, pression)
- [ ] Application mobile (React Native/Flutter)
- [ ] Base de données PostgreSQL/MySQL

### Long terme
- [ ] Machine Learning pour prédictions
- [ ] Intégration avec assistants vocaux
- [ ] Support multi-dispositifs
- [ ] Déploiement cloud (AWS/Azure)

## Dépannage

### Problèmes courants

**ESP8266 ne se connecte pas au Wi-Fi**
```cpp
// Vérifier les identifiants Wi-Fi
// Redémarrer le routeur
// Vérifier la portée du signal
```

**Données non reçues par Django**
```bash
# Vérifier l'URL du serveur
# Tester avec curl :
curl -X POST http://localhost:8000/api/recevoir/ \
     -H "Content-Type: application/json" \
     -d '{"temperature": 25, "humidity": 60}'
```

**Erreurs de capteur DHT11**
```cpp
// Vérifier les connexions
// Attendre 2 secondes entre lectures
// Remplacer le capteur si défaillant
```

## Licence

Ce projet est sous licence MIT. Voir le fichier `LICENSE` pour plus de détails.

## Avertissements

- Ce projet est à des fins éducatives
- Sécurisez vos credentials Wi-Fi et API
- Calibrez régulièrement vos capteurs
- Vérifiez la stabilité de votre connexion réseau

## Support

Pour toute question ou problème :
- 📧 Email : daoudaba4500@gmail.com

---

**Développé avec passion pour l'IoT et la surveillance environnementale** 🌱

=======
⭐ Si ce projet vous aide, n'hésitez pas à me donner une étoile!
