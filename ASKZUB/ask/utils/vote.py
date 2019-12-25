from ask.models import QuestionVote, Question, Author
from django.db.models import F


def add_vote(qid, uid, value):
    if value == "dec":
        like_value = 0
    else:
        like_value = 1

    is_update_rating_need = False

    qv = QuestionVote.objects.filter(question_id=qid, author_id=uid)
    if qv.exists():
        print("exists")
        is_update_rating_need = qv[0].value != like_value
        QuestionVote.objects.filter(question_id=qid, author_id=uid).update(value=like_value)
    else:
        print("not exists")
        QuestionVote.objects.create(question_id=qid,
                                    author_id=uid,
                                    value=like_value)
        is_update_rating_need = True

    if is_update_rating_need:
        if like_value == 0:
            like_value = -1

        Question.objects.filter(pk=qid).update(rating=F('rating') + like_value)
        Author.objects.filter(pk=Question.objects.get(pk=qid).author_id).update(rating=F('rating') + like_value)

    return Question.objects.get(pk=qid).rating
