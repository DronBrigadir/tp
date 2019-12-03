from django import forms
from django.contrib.auth.models import User
from django.contrib.auth import authenticate
from ask.models import Question, Author, Tag, Answer


class LoginForm(forms.ModelForm):
    class Meta:
        model = User
        fields = ['username', 'password']

        widgets = {
            'username': forms.TextInput(attrs={'class': 'form-control col-sm-4'}),
            'password': forms.PasswordInput(attrs={'class': 'form-control col-sm-4'})
        }

    def clean(self):
        username = self.cleaned_data.get('username')
        password = self.cleaned_data.get('password')
        user = authenticate(username=username, password=password)
        if not user or not user.is_active:
            raise forms.ValidationError("Login is invalid. Please, try again")
        return self.cleaned_data


class QuestionForm(forms.ModelForm):
    tags = forms.CharField(required=False, max_length=128,
                           widget=forms.TextInput(attrs={'class': 'form-control col-sm-8',
                                                         'placeholder': 'tag1, tag2, tag3...'}))

    class Meta:
        model = Question
        fields = ['title', 'content']

        widgets = {
            'title': forms.TextInput(attrs={'class': 'form-control col-sm-8'}),
            'content': forms.Textarea(attrs={'class': 'form-control col-sm-8'})
        }

    def __init__(self, user, *args, **kwargs):
        self.user = user
        super().__init__(*args, **kwargs)

    def save(self, commit=True):
        obj = super().save(commit=False)
        author_id = Author.objects.get(user_id=self.user.pk).id
        obj.author_id = author_id
        if commit:
            obj.save()

        tags = self.cleaned_data.get('tags')
        tags_list = [x.lower().strip() for x in tags.split(',') if x]

        for tag in tags_list:
            if Tag.objects.filter(name=tag).exists():
                t = Tag.objects.get(name=tag)
                obj.tag_set.add(t)
            else:
                t = Tag.objects.create(name=tag)
                t.save()
                obj.tag_set.add(t)

        return obj


class AnswerForm(forms.ModelForm):
    class Meta:
        model = Answer
        fields = ['content']

        labels = {
            'content': 'Your Answer'
        }

        widgets = {
            'content': forms.Textarea(attrs={'class': 'form-control', 'cols': '100%', 'rows': '5'})
        }

    def __init__(self, user, question_id, *args, **kwargs):
        self.user = user
        self.question_id = question_id
        super().__init__(*args, **kwargs)

    def save(self, commit=True):
        obj = super().save(commit=False)
        obj.author_id = Author.objects.get(user_id=self.user.pk).id
        obj.question_id = self.question_id

        if commit:
            obj.save()


class RegistrationForm(forms.ModelForm):
    first_name = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control col-sm-6',
                                                               'required': True}))
    last_name = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control col-sm-6',
                                                              'required': True}))
    email = forms.CharField(widget=forms.EmailInput(attrs={'class': 'form-control col-sm-6',
                                                           'required': True}))
    username = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control col-sm-6',
                                                             'required': True}))
    password = forms.CharField(widget=forms.PasswordInput(attrs={'class': 'form-control col-sm-6',
                                                                 'required': True}))
    confirm_password = forms.CharField(widget=forms.PasswordInput(attrs={'class': 'form-control col-sm-6',
                                                                         'required': True}))

    class Meta:
        model = Author
        fields = ['avatar']

    field_order = [
            'first_name',
            'last_name',
            'email',
            'username',
            'password',
            'confirm_password',
            'avatar'
        ]

    def clean(self):
        confirm_password = self.cleaned_data.get('confirm_password')
        password = self.cleaned_data.get('password')

        if password != confirm_password:
            raise forms.ValidationError('Passwords do not match')

    def save(self, commit=True):
        u = User.objects.create(username=self.cleaned_data.get('username'),
                                email=self.cleaned_data.get('email'),
                                first_name=self.cleaned_data.get('first_name'),
                                last_name=self.cleaned_data.get('last_name'))
        u.set_password(self.cleaned_data.get('password'))
        u.save()

        a = super().save(commit=False)
        a.user = u

        if commit:
            a.save()

        return u


class ProfileForm(forms.ModelForm):
    class Meta:
        model = User
        fields = ['first_name', 'last_name', 'email', 'username']
        widgets = {
            'username': forms.TextInput(attrs={'class': 'form-control col-sm-6', 'readonly': True}),
            'email': forms.EmailInput(attrs={'class': 'form-control col-sm-6', 'readonly': True}),
            'first_name': forms.TextInput(attrs={'class': 'form-control col-sm-6', 'readonly': True}),
            'last_name': forms.TextInput(attrs={'class': 'form-control col-sm-6', 'readonly': True})
        }
