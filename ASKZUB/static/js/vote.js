function getCookie(name) {
    var cookieValue = null;
    if (document.cookie && document.cookie !== '') {
        var cookies = document.cookie.split(';');
        for (var i = 0; i < cookies.length; i++) {
            var cookie = cookies[i].trim();
            // Does this cookie string begin with the name we want?
            if (cookie.substring(0, name.length + 1) === (name + '=')) {
                cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                break;
            }
        }
    }
    return cookieValue;
}

var csrftoken = getCookie('csrftoken');


$('.mx-auto').on(
    'click', function() {
        el = $(this);

        qid = el.data('qid');
        value = el.data('value');
        uid = el.data('uid')
        data = {qid: qid, value: value, uid: uid};
        console.log("DATA", data);

        fetch(
            '/vote/', {
                method: 'POST',
                body: JSON.stringify(data),
                credentials: 'include',
                headers: { "X-CSRFToken": csrftoken },
            }
        )
            .then(response => response.json())
            .then(resp_data => {
                console.log("RESPONCE", resp_data);
                let rating_elem = document.getElementById(qid)
                rating_elem.innerHTML = resp_data["current_question_rating"]
            });

        return false;
    }
);