from django.shortcuts import render
from django.http import HttpResponseRedirect
from django.contrib.auth import authenticate, login
from django.contrib.auth import logout as django_logout
from django.contrib.auth.mixins import LoginRequiredMixin
from django.contrib.auth.decorators import login_required
from django.utils.decorators import method_decorator
from django.views.generic import View
from django.urls import reverse
from ask.models import Tag, Author, Question
from ask.utils.paginator import paginate
from ask.forms import LoginForm, QuestionForm, AnswerForm, RegistrationForm, ProfileForm


def index(request):
    page_number = request.GET.get('page', 1)
    limit = request.GET.get('limit', 5)
    context = {
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
        next_url = request.GET.get('next', reverse('ask:index'))
        form = LoginForm()
        context = {
            'form': form,
            'next_url': next_url
        }
        return render(request, 'login.html', context)

    def post(self, request):
        next_url = request.GET.get('next', reverse('ask:index'))
        form = LoginForm(data=request.POST)

        if form.is_valid():
            username = form.cleaned_data.get('username')
            password = form.cleaned_data.get('password')
            user = authenticate(username=username, password=password)

            if user is not None:
                login(request, user)
                return HttpResponseRedirect(next_url)
        else:
            context = {
                'next_url': next_url,
                'form': form
            }
            return render(request, 'login.html', context)


def logout(request):
    django_logout(request)
    url = request.META.get('HTTP_REFERER')
    if url is None:
        url = reverse('ask:index')
    return HttpResponseRedirect(url)


class SignUpView(View):
    def get(self, request, **kwargs):
        form = kwargs.get('form', RegistrationForm())
        context = {
            'form': form
        }
        return render(request, 'signup.html', context)

    def post(self, request):
        form = RegistrationForm(data=request.POST, files=request.FILES)

        if form.is_valid():
            user = form.save()
            login(request, user)
            return HttpResponseRedirect(reverse('ask:index'))
        else:
            return self.get(request, form=form)


class AskView(LoginRequiredMixin, View):
    def get(self, request):
        form = QuestionForm(request.user)
        context = {
            'form': form
        }
        return render(request, 'ask.html', context)

    def post(self, request):
        form = QuestionForm(request.user, data=request.POST)

        if form.is_valid():
            q = form.save()
            return HttpResponseRedirect(reverse('ask:question', kwargs={'question_id': q.pk}))
        else:
            context = {
                'form': form
            }
            return render(request, 'ask.html', context)


class QuestionView(View):
    def get(self, request, question_id, **kwargs):
        q = Question.objects.by_id(question_id)
        page_number = request.GET.get('page', 1)
        limit = request.GET.get('limit', 5)
        form = kwargs.get('form', AnswerForm(request.user, question_id))
        context = {
            'question': q,
            'answers': paginate(q.answer_set.all(), limit, page_number),
            'form': form
        }
        return render(request, 'question.html', context)

    @method_decorator(login_required)
    def post(self, request, question_id):
        form = AnswerForm(request.user, question_id, data=request.POST)

        if form.is_valid():
            form.save()
            return HttpResponseRedirect(reverse('ask:question', kwargs={'question_id': question_id}))
        else:
            return self.get(request, question_id, form=form)


def tag(request, tag_name):
    page_number = request.GET.get('page', 1)
    limit = request.GET.get('limit', 5)
    context = {
        'tag_name': tag_name,
        'questions_to_show': Question.objects.questions_by_tag(tag_name, page_number, limit)
    }
    return render(request, 'tag.html', context)


@login_required
def profile(request):
    form = ProfileForm(instance=request.user)
    context = {
        'form': form
    }
    return render(request, 'profile.html', context)
