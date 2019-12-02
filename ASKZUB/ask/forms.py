from django import forms
from django.contrib.auth.models import User
from django.contrib.auth import authenticate
from ask.models import Question, Author, Tag


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
                           widget=forms.TextInput(attrs={'class': 'form-control col-sm-8'}))

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
        tags_list = tags.split(',')
        tags_list = [x.lower() for x in tags_list]

        for tag in tags_list:
            if Tag.objects.filter(name=tag).exists():
                t = Tag.objects.get(name=tag)
                obj.tag_set.add(t)
            else:
                t = Tag.objects.create(name=tag)
                t.save()
                obj.tag_set.add(t)

        return obj
