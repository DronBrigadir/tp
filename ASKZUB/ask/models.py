from django.db import models
from django.db.models import Count
from django.conf import settings
from django.utils import timezone


class AuthorManager(models.Manager):
    def best(self):
        return self.order_by('-rating')[:3]


class Author(models.Model):
    user = models.OneToOneField(settings.AUTH_USER_MODEL,
                                on_delete=models.CASCADE)
    rating = models.IntegerField(default=0)
    avatar = models.ImageField(upload_to='images/avatars/{0}'.format(user.db_index),
                               default='images/avatars/default_avatar.svg')

    def __str__(self):
        return self.user.username

    object = AuthorManager()


class QuestionManager(models.Manager):
    def recent(self):
        self.order_by('-creation_time')


class Question(models.Model):
    author = models.ForeignKey('Author', on_delete=models.CASCADE)
    title = models.CharField(max_length=128)
    content = models.TextField()
    votes = models.IntegerField(default=0)
    creation_time = models.DateTimeField(default=timezone.now)

    def __str__(self):
        return self.title

    object = QuestionManager()


class Answer(models.Model):
    author = models.ForeignKey('Author', on_delete=models.CASCADE)
    question = models.ForeignKey('Question', on_delete=models.CASCADE)
    votes = models.IntegerField(default=0)
    content = models.TextField
    creation_time = models.DateTimeField(default=timezone.now)

    def __str__(self):
        return "Answer to question: {}".format(self.question.title)


class TagManager(models.Manager):
    def popular(self):
        return self.annotate(question_count=Count('question')).order_by('-question_count')[:3]


class Tag(models.Model):
    question = models.ManyToManyField(Question)
    name = models.CharField(max_length=32)

    def __str__(self):
        return self.name

    object = TagManager()
