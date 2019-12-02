from django.urls import path
from . import views
from django.conf import settings
from django.conf.urls.static import static

app_name = 'ask'
urlpatterns = [
    path('', views.index, name='index'),
    path('hot/', views.hot, name='hot'),
    path('login/', views.LoginView.as_view(), name='login'),
    path('signup/', views.signup, name='signup'),
    path('ask/', views.AskView.as_view(), name='ask'),
    path('question/<int:question_id>/', views.QuestionView.as_view(), name='question'),
    path('tag/<str:tag_name>/', views.tag, name='tag'),
    path('logout/', views.logout, name='logout')
]
urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
