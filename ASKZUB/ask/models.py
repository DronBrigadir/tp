from django.db import models
from django.db.models import Count
from django.conf import settings
from django.utils import timezone
from ask.utils import paginator


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

    objects = AuthorManager()


class QuestionManager(models.Manager):
    def recent(self, page_number):
        return paginator.paginate(self.order_by('-creation_time'), 4, page_number)

    def hot(self, page_number):
        return paginator.paginate(self.order_by('-rating'), 4, page_number)

    def by_id(self, question_id):
        return self.get(pk=question_id)


class Question(models.Model):
    author = models.ForeignKey('Author', on_delete=models.CASCADE)
    title = models.CharField(max_length=128)
    content = models.TextField(null=False)
    rating = models.IntegerField(default=0)
    creation_time = models.DateTimeField(default=timezone.now)

    def __str__(self):
        return self.title

    objects = QuestionManager()


class AnswerManager(models.Manager):
    def for_question(self, question_id, page_number, question):
        object_list = self.filter(question=question).order_by('-creation_time')
        return paginator.paginate(object_list, 2, page_number)


class Answer(models.Model):
    author = models.ForeignKey('Author', on_delete=models.CASCADE)
    question = models.ForeignKey('Question', on_delete=models.CASCADE)
    rating = models.IntegerField(default=0)
    content = models.TextField(default='Answer on question')
    creation_time = models.DateTimeField(default=timezone.now)

    def __str__(self):
        return "Answer to question: {}".format(self.question.title)

    objects = AnswerManager()


class TagManager(models.Manager):
    def popular(self):
        return self.annotate(question_count=Count('question')).order_by('-question_count')[:3]


class Tag(models.Model):
    question = models.ManyToManyField(Question)
    name = models.CharField(max_length=32, unique=True)

    def __str__(self):
        return self.name

    objects = TagManager()


class QuestionVote(models.Model):
    value = models.SmallIntegerField(default=0)
    question = models.ForeignKey(Question, on_delete=models.CASCADE)
    author = models.ForeignKey(Author, on_delete=models.CASCADE)

    def __str__(self):
        return "{0} votes to question: {1}".format(self.author,
                                                   self.question.author)


class AnswerVote(models.Model):
    value = models.SmallIntegerField(default=0)
    answer = models.ForeignKey(Answer, on_delete=models.CASCADE)
    author = models.ForeignKey(Author, on_delete=models.CASCADE)

    def __str__(self):
        return "{0} votes to answer: {1}".format(self.author,
                                                 self.answer)
