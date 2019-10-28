from django.shortcuts import render
from ask.models import Tag, Author, Question, Answer
from ask.utils import paginator


def index(request):
    question_page_number = request.GET.get('page')
    if question_page_number is None:
        question_page_number = 1
    context = {
        'popular_tags': Tag.objects.popular(),
        'best_members': Author.objects.best(),
        #'question_to_show': Question.objects.recent(question_page_number)
        'questions_to_show': paginator.paginate(Question.objects.order_by('-creation_time'), 4, question_page_number)
    }
    return render(request, 'index.html', context)


def login(request):
    return render(request, 'login.html')


def signup(request):
    return render(request, 'signup.html')


def ask(request):
    return render(request, 'ask.html')


def question(request, question_id):
    q = Question.objects.by_id(question_id)
    answer_page_number = request.GET.get('page')
    context = {
        'popular_tags': Tag.objects.popular(),
        'best_members': Author.objects.best(),
        'question': q,
        'answers': paginator.paginate(Answer.objects.filter(question=q).order_by('-creation_time'), 2, answer_page_number)
        #'answers': Answer.objects.for_question(request, question_id)
    }
    return render(request, 'question.html', context)
