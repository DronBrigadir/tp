from django.urls import path
from . import views
from django.conf import settings
from django.conf.urls.static import static

app_name = 'ask'
urlpatterns = [
    path('', views.index, name='index'),
    path('login/', views.login, name='login'),
    path('signup/', views.signup, name='signup'),
    path('ask/', views.ask, name='ask'),
    path('question/', views.question, name='question'),
]
urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)