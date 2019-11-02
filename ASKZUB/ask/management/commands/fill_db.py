from django.contrib.auth.models import User
from django.core.management.base import BaseCommand
from django.db.models import Sum
from faker import Faker
from random import choice
from pytz import timezone

from ask.models import Author, Question, Answer, Tag, QuestionVote, AnswerVote

f = Faker()


class Command(BaseCommand):

    USERS_COUNT = 3
    TAGS_COUNT = 3
    QUESTIONS_COUNT_FOR_ONE_USER = 3
    ANSWERS_COUNT_FOR_ONE_QUESTION = 3

    def authors_generator(self):
        for _ in range(0, self.USERS_COUNT):
            u = User.objects.create(username=f.name(),
                                    password=f.password(),
                                    email=f.free_email())
            u.save()
            a = Author.objects.create(user=u)
            a.save()

    def question_generator(self):
        authors = Author.objects.all()
        moscow_tz = timezone('Europe/Moscow')
        for a in authors:
            for _ in range(0, self.QUESTIONS_COUNT_FOR_ONE_USER):
                q = Question.objects.create(author=a,
                                            title=f.text(16),
                                            content=f.text(256),
                                            creation_time=f.date_time_this_year(tzinfo=moscow_tz))
                q.save()

    def answers_generator(self):
        authors = Author.objects.all()
        questions = Question.objects.all()
        moscow_tz = timezone('Europe/Moscow')
        for q in questions:
            for _ in range(0, self.ANSWERS_COUNT_FOR_ONE_QUESTION):
                a = Answer.objects.create(author=choice(authors),
                                          question=q,
                                          content=f.text(256),
                                          creation_time=f.date_time_between(start_date=q.creation_time,
                                                                            end_date='+30d',
                                                                            tzinfo=moscow_tz))
                a.save()

    def tags_generator(self):
        questions = Question.objects.all()
        for _ in range(0, self.TAGS_COUNT):
            t = Tag.objects.create(name=f.color_name())
            for _ in range(0, self.QUESTIONS_COUNT_FOR_ONE_USER):
                t.question.add(choice(questions))
            t.save()

    def votes_generator(self):
        authors = Author.objects.all()
        questions = Question.objects.all()
        answers = Answer.objects.all()

        for u in authors:
            for q in questions:
                qv = QuestionVote.objects.create(author=u,
                                                 question=q,
                                                 value=f.random_int(min=0, max=1))
                qv.save()

            for a in answers:
                av = AnswerVote.objects.create(author=u,
                                               answer=a,
                                               value=f.random_int(min=0, max=1))
                av.save()

        for q in questions:
            likes = QuestionVote.objects.filter(question=q, value=1).count()
            dislikes = QuestionVote.objects.filter(question=q, value=0).count()
            Question.objects.filter(pk=q.pk).update(rating=likes - dislikes)

        for a in answers:
            likes = AnswerVote.objects.filter(answer=a, value=1).count()
            dislikes = AnswerVote.objects.filter(answer=a, value=0).count()
            Answer.objects.filter(pk=a.pk).update(rating=likes - dislikes)

        for a in authors:
            qvotes = Question.objects.filter(author=a).aggregate(Sum('rating'))
            avotes = Answer.objects.filter(author=a).aggregate(Sum('rating'))
            Author.objects.filter(pk=a.pk).update(rating=qvotes['rating__sum'] + avotes['rating__sum'])

    def generator(self):
        self.authors_generator()
        self.question_generator()
        self.answers_generator()
        self.tags_generator()
        self.votes_generator()

    def handle(self, *args, **options):
        self.generator()
