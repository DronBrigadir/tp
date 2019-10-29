from django.shortcuts import render
from ask.models import Tag, Author, Question, Answer


def index(request):
    question_page_number = request.GET.get('page')
    if question_page_number is None:
        question_page_number = 1
    context = {
        'popular_tags': Tag.objects.popular(),
        'best_members': Author.objects.best(),
        'questions_to_show': Question.objects.recent(question_page_number)
    }
    return render(request, 'index.html', context)


def login(request):
    context = {
        'popular_tags': Tag.objects.popular(),
        'best_members': Author.objects.best()
    }
    return render(request, 'login.html', context)


def signup(request):
    context = {
        'popular_tags': Tag.objects.popular(),
        'best_members': Author.objects.best()
    }
    return render(request, 'signup.html', context)


def ask(request):
    context = {
        'popular_tags': Tag.objects.popular(),
        'best_members': Author.objects.best()
    }
    return render(request, 'ask.html', context)


def question(request, question_id):
    q = Question.objects.by_id(question_id)
    answer_page_number = request.GET.get('page')
    if answer_page_number is None:
        answer_page_number = 1
    context = {
        'popular_tags': Tag.objects.popular(),
        'best_members': Author.objects.best(),
        'question': q,
        'answers': Answer.objects.for_question(question_id, answer_page_number, q)
    }
    return render(request, 'question.html', context)
