from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
import json
from .models import Mesure
from django.shortcuts import render 
import sib_api_v3_sdk
from sib_api_v3_sdk.rest import ApiException 



@csrf_exempt
# def recevoir_donnees(request):
#     if request.method == 'POST':
#         try:
#             data = json.loads(request.body)
#             temp = data.get('temperature')
#             hum = data.get('humidity')
#             if temp is not None and hum is not None:
#                 Mesure.objects.create(temperature=temp, humidite=hum)
#                 return JsonResponse({'status': 'ok'})
#         except:
#             return JsonResponse({'error': 'Invalid JSON'}, status=400)
#     return JsonResponse({'error': 'Method not allowed'}, status=405)


def recevoir_donnees(request):
    if request.method == 'POST':
        try:
            data = json.loads(request.body)
            temp = data.get('temperature')
            hum = data.get('humidity')
            if temp is not None and hum is not None:
                Mesure.objects.create(temperature=temp, humidite=hum)

                #  Alerte si température élevée
                if temp > 34:
                    envoyer_mail_brevo(temp, hum)

                return JsonResponse({'status': 'ok'})
        except:
            return JsonResponse({'error': 'Invalid JSON'}, status=400)
    return JsonResponse({'error': 'Method not allowed'}, status=405)




def dashboard(request):
    mesures = Mesure.objects.order_by('-timestamp')[:20]
    return render(request, 'capteurs/dashboard.html', {'mesures': mesures})


def envoyer_mail_brevo(temp, hum):
    configuration = sib_api_v3_sdk.Configuration()
    configuration.api_key['api-key'] = 'your_api_key'  # remplace par ta clé réelle

    api_instance = sib_api_v3_sdk.TransactionalEmailsApi(sib_api_v3_sdk.ApiClient(configuration))
    email = sib_api_v3_sdk.SendSmtpEmail(
        to=[{"email": "your_email@example.com"}],
        subject="🚨 Alerte Température Élevée",
        html_content=f"<h3>Attention !</h3><p>Température mesurée : {temp} °C<br>Humidité : {hum} %</p>",
        sender={"name": "Système IoT", "email": "your_email@example.com"}
    )

    try:
        api_instance.send_transac_email(email)
    except ApiException as e:
        print("Erreur d'envoi email Brevo :", e)

