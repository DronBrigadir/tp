from django.contrib import admin
from .models import Author
from .models import Question
from .models import Answer
from .models import Tag

admin.site.register(Author)
admin.site.register(Question)
admin.site.register(Answer)
admin.site.register(Tag)