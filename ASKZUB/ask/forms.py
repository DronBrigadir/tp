from django import forms
from django.contrib.auth.models import User
from django.contrib.auth import authenticate
from django.core.validators import validate_email
from ask.models import Question, Author, Tag, Answer


class LoginForm(forms.ModelForm):
    class Meta:
        model = User
        fields = ['username', 'password']

        widgets = {
            'username': forms.TextInput(attrs={'class': 'form-control col-sm-6'}),
            'password': forms.PasswordInput(attrs={'class': 'form-control col-sm-6'})
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

    def is_valid(self):
        ret = forms.Form.is_valid(self)
        for f in self.errors:
            if f != "__all__":
                self.fields[f].widget.attrs.update(
                    {'class': self.fields[f].widget.attrs.get('class', '') + ' is-invalid'})
        return ret

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

    def is_valid(self):
        ret = forms.Form.is_valid(self)
        for f in self.errors:
            if f != "__all__":
                self.fields[f].widget.attrs.update(
                    {'class': self.fields[f].widget.attrs.get('class', '') + ' is-invalid'})
        return ret

    def save(self, commit=True):
        obj = super().save(commit=False)
        obj.author_id = Author.objects.get(user_id=self.user.pk).id
        obj.question_id = self.question_id

        if commit:
            obj.save()


class RegistrationForm(forms.ModelForm):
    first_name = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control col-sm-6'}))
    last_name = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control col-sm-6'}))
    email = forms.CharField(widget=forms.EmailInput(attrs={'class': 'form-control col-sm-6'}),
                            validators=[validate_email])
    username = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control col-sm-6'}))
    password = forms.CharField(widget=forms.PasswordInput(attrs={'class': 'form-control col-sm-6'}))
    confirm_password = forms.CharField(widget=forms.PasswordInput(attrs={'class': 'form-control col-sm-6'}))

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

    def is_valid(self):
        ret = forms.Form.is_valid(self)
        for f in self.errors:
            if f != "__all__":
                self.fields[f].widget.attrs.update(
                    {'class': self.fields[f].widget.attrs.get('class', '') + ' is-invalid'})
        return ret

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
    first_name = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control col-sm-6'}))
    last_name = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control col-sm-6'}))
    email = forms.CharField(widget=forms.EmailInput(attrs={'class': 'form-control col-sm-6'}),
                            validators=[validate_email])
    username = forms.CharField(widget=forms.TextInput(attrs={'class': 'form-control col-sm-6'}))
    password = forms.CharField(label='New password',
                               widget=forms.PasswordInput(attrs={'class': 'form-control col-sm-6'}),
                               required=False)
    confirm_password = forms.CharField(widget=forms.PasswordInput(attrs={'class': 'form-control col-sm-6'}),
                                       required=False)

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

    def __init__(self, *args, **kwargs):
        instance = kwargs.pop('instance', {})
        super(ProfileForm, self).__init__(instance=instance.author, *args, **kwargs)
        self.fields['first_name'].initial = instance.first_name
        self.fields['last_name'].initial = instance.last_name
        self.fields['email'].initial = instance.email
        self.fields['username'].initial = instance.username

    def is_valid(self):
        ret = forms.Form.is_valid(self)
        for f in self.errors:
            if f != "__all__":
                self.fields[f].widget.attrs.update(
                    {'class': self.fields[f].widget.attrs.get('class', '') + ' is-invalid'})
        return ret

    def clean(self):
        confirm_password = self.cleaned_data.get('confirm_password')
        password = self.cleaned_data.get('password')

        if password != confirm_password:
            raise forms.ValidationError('Passwords do not match')

    def save(self, commit=True):
        author = super().save(commit=True)
        user = author.user
        user.first_name = self.cleaned_data.get('first_name')
        user.last_name = self.cleaned_data.get('last_name')
        user.email = self.cleaned_data.get('email')
        user.username = self.cleaned_data.get('username')
        password = self.cleaned_data.get('password')
        if password:
            user.set_password(password)
        user.save(update_fields=['first_name', 'last_name', 'email', 'username', 'password'])
