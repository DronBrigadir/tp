from django.contrib.auth.models import User
from django.core.management.base import BaseCommand
from django.db.models import Sum
from faker import Faker
from random import choice
from pytz import timezone
import random

from ask.models import Author, Question, Answer, Tag, QuestionVote, AnswerVote

f = Faker()


class Command(BaseCommand):

    USERS_COUNT = 10
    TAGS_COUNT = 10
    QUESTIONS_COUNT_FOR_ONE_USER = 5
    MAX_ANSWERS_FOR_ONE_QUESTION = 5
    TAGS_COUNT_FOR_ONE_QUESTION = 3

    def authors_generator(self):
        for _ in range(0, self.USERS_COUNT):
            u = User.objects.create(username=f.user_name(),
                                    email=f.free_email(),
                                    first_name=f.first_name(),
                                    last_name=f.last_name())
            u.set_password(f.password())
            u.save()
            a = Author.objects.create(user=u)
            a.save()

    def question_generator(self):
        author_ids = list(Author.objects.values_list('id', flat=True))
        moscow_tz = timezone('Europe/Moscow')
        for a_id in author_ids:
            for _ in range(0, self.QUESTIONS_COUNT_FOR_ONE_USER):
                q = Question.objects.create(author_id=a_id,
                                            title=f.text(16),
                                            content=f.text(256),
                                            creation_time=f.date_time_this_year(tzinfo=moscow_tz))
                q.save()

    def answers_generator(self):
        author_ids = list(Author.objects.values_list('id', flat=True))
        question_ids = list(Question.objects.values_list('id', flat=True))
        moscow_tz = timezone('Europe/Moscow')
        for q_id in question_ids:
            for _ in range(0, random.randint(1, self.MAX_ANSWERS_FOR_ONE_QUESTION)):
                a = Answer.objects.create(author_id=choice(author_ids),
                                          question_id=q_id,
                                          content=f.text(256),
                                          creation_time=
                                          f.date_time_between(start_date=Question.objects.get(pk=q_id).creation_time,
                                                              end_date='+30d',
                                                              tzinfo=moscow_tz))
                a.save()

    def tags_generator(self):
        for _ in range(0, self.TAGS_COUNT):
            name = f.color_name()
            while Tag.objects.filter(name=name).exists():
                name = f.color_name()

            t = Tag.objects.create(name=name)
            t.save()

        question_ids = list(Question.objects.values_list('id', flat=True))
        tag_ids = list(Tag.objects.values_list('id', flat=True))
        for q_id in question_ids:
            for _ in range(0, self.TAGS_COUNT_FOR_ONE_QUESTION):
                Question.objects.get(pk=q_id).tag_set.add(Tag.objects.get(pk=choice(tag_ids)))

    def votes_generator(self):
        author_ids = list(Author.objects.values_list('id', flat=True))
        question_ids = list(Question.objects.values_list('id', flat=True))
        answer_ids = list(Answer.objects.values_list('id', flat=True))

        for author_id in author_ids:
            for q_id in question_ids:
                if Question.objects.get(pk=q_id).author_id != author_id:
                    qv = QuestionVote.objects.create(author_id=author_id,
                                                     question_id=q_id,
                                                     value=f.random_int(min=0, max=1))
                    qv.save()

            for answer_id in answer_ids:
                if Answer.objects.get(pk=answer_id).author_id != author_id:
                    av = AnswerVote.objects.create(author_id=author_id,
                                                   answer_id=answer_id,
                                                   value=f.random_int(min=0, max=1))
                    av.save()

        for q_id in question_ids:
            likes = QuestionVote.objects.filter(question_id=q_id, value=1).count()
            dislikes = QuestionVote.objects.filter(question_id=q_id, value=0).count()
            Question.objects.filter(pk=q_id).update(rating=likes - dislikes)

        for answer_id in answer_ids:
            likes = AnswerVote.objects.filter(answer_id=answer_id, value=1).count()
            dislikes = AnswerVote.objects.filter(answer_id=answer_id, value=0).count()
            Answer.objects.filter(pk=answer_id).update(rating=likes - dislikes)

        for author_id in author_ids:
            qvotes = Question.objects.filter(author_id=author_id).aggregate(Sum('rating'))
            avotes = Answer.objects.filter(author_id=author_id).aggregate(Sum('rating'))
            Author.objects.filter(pk=author_id).update(rating=qvotes['rating__sum'] + avotes['rating__sum'])

    def generator(self):
        self.authors_generator()
        self.question_generator()
        self.answers_generator()
        self.tags_generator()
        self.votes_generator()

    def handle(self, *args, **options):
        self.generator()
