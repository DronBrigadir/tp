from django.core.paginator import Paginator


def paginate(objects_list, limit, page_number):
    try:
        limit = int(limit)
    except ValueError:
        limit = 5
    if limit > 20:
        limit = 5

    paginator = Paginator(objects_list, limit)
    return paginator.get_page(page_number)
