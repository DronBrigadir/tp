from django.db import models
from django.conf import settings
from django.utils import timezone


class Author(models.Model):
    user = models.OneToOneField(settings.AUTH_USER_MODEL,
                                on_delete=models.CASCADE)
    rating = models.IntegerField(default=0)

    def __str__(self):
        return self.user.username


class Question(models.Model):
    author = models.ForeignKey('Author', on_delete=models.CASCADE)
    title = models.CharField(max_length=128)
    content = models.TextField()
    votes = models.IntegerField(default=0)
    creation_time = models.DateTimeField(default=timezone.now)

    def __str__(self):
        return self.title


class Answer(models.Model):
    author = models.ForeignKey('Author', on_delete=models.CASCADE)
    question = models.ForeignKey('Question', on_delete=models.CASCADE)
    votes = models.IntegerField(default=0)
    content = models.TextField
    creation_time = models.DateTimeField(default=timezone.now)

    def __str__(self):
        return "Answer to question: {}".format(self.question.title)


class Tag(models.Model):
    question = models.ManyToManyField(Question)
    name = models.CharField(max_length=32)

    def __str__(self):
        return self.name
