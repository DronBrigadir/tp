from django.core.paginator import Paginator


def paginate(objects_list, items_per_page, page_number):
    paginator = Paginator(objects_list, items_per_page)
    return paginator.get_page(page_number)
