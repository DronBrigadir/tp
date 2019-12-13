from ask.models import Tag, Author


def tag(request):
    return {
        'popular_tags': Tag.objects.popular()
    }


def member(request):
    return {
        'best_members': Author.objects.best()
    }
