from django.core.paginator import Paginator


def paginate(objects_list, items_per_page, request):
    paginator = Paginator(objects_list, items_per_page)
    page = request.GET.get("page")
    return paginator.get_page(page)
