from django.shortcuts import render
from django.http import HttpResponseRedirect
from django.contrib.auth import authenticate, login
from django.views.generic import View
from django.urls import reverse
from ask.models import Tag, Author, Question
from ask.utils.paginator import paginate
from ask.forms import LoginForm


def index(request):
    page_number = request.GET.get('page', 1)
    limit = request.GET.get('limit', 5)
    context = {
        'popular_tags': Tag.objects.popular(),
        'best_members': Author.objects.best(),
        'questions_to_show': Question.objects.recent(page_number, limit),
        'questions_switcher': {
            'title': 'Hot Questions',
            'href': '/hot'
        },
        'title': 'New Questions'
    }
    return render(request, 'index.html', context)


def hot(request):
    page_number = request.GET.get('page', 1)
    limit = request.GET.get('limit', 5)
    context = {
        'popular_tags': Tag.objects.popular(),
        'best_members': Author.objects.best(),
        'questions_to_show': Question.objects.hot(page_number, limit),
        'questions_switcher': {
            'title': 'New Questions',
            'href': '/'
        },
        'title': 'Hot Questions'
    }
    return render(request, 'index.html', context)


class LoginView(View):
    def get(self, request):
        form = LoginForm()
        context = {
            'popular_tags': Tag.objects.popular(),
            'best_members': Author.objects.best(),
            'form': form
        }
        return render(request, 'login.html', context)

    def post(self, request):
        form = LoginForm(data=request.POST)
        context = {
            'popular_tags': Tag.objects.popular(),
            'best_members': Author.objects.best(),
        }

        if form.is_valid():
            username = form.cleaned_data.get('username')
            password = form.cleaned_data.get('password')
            user = authenticate(username=username, password=password)

            if user is not None:
                login(request, user)
                return HttpResponseRedirect(reverse('ask:index'), context)
        else:
            context['form'] = form
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
    page_number = request.GET.get('page', 1)
    limit = request.GET.get('limit', 5)
    context = {
        'popular_tags': Tag.objects.popular(),
        'best_members': Author.objects.best(),
        'question': q,
        'answers': paginate(q.answer_set.all(), limit, page_number)
    }
    return render(request, 'question.html', context)


def tag(request, tag_name):
    page_number = request.GET.get('page', 1)
    limit = request.GET.get('limit', 5)
    context = {
        'tag_name': tag_name,
        'popular_tags': Tag.objects.popular(),
        'best_members': Author.objects.best(),
        'questions_to_show': Question.objects.questions_by_tag(tag_name, page_number, limit)
    }
    return render(request, 'tag.html', context)
