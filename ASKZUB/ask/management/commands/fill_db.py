from django.contrib.auth.models import User
from django.core.management.base import BaseCommand
from faker import Faker
from random import choice
from pytz import timezone

from ask.models import Author, Question, Answer, Tag

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
            a = Author.objects.create(user=u,
                                      rating=f.random_int(min=-100, max=100))
            a.save()

    def question_generator(self):
        authors = Author.objects.all()
        moscow_tz = timezone('Europe/Moscow')
        for a in authors:
            for _ in range(0, self.QUESTIONS_COUNT_FOR_ONE_USER):
                q = Question.objects.create(author=a,
                                            title=f.text(16),
                                            content=f.text(256),
                                            votes=f.random_int(min=-100, max=100),
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
                                          votes=f.random_int(min=-100, max=100),
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

    def generator(self):
        self.authors_generator()
        self.question_generator()
        self.answers_generator()
        self.tags_generator()

    def handle(self, *args, **options):
        self.generator()
