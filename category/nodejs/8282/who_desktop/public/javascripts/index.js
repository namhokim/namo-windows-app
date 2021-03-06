//jQuery Script for index.html
function pad2(number) {  
	return (number < 10 ? '0' : '') + number;
}
function getCurrentTime() {
	var now = new Date();
	return pad2(now.getHours()) + ':' + pad2(now.getMinutes()) + ':' + pad2(now.getSeconds());
}

$(function () {		// same as $(document).ready

	// socket.io /////////////////////////////////////////////////////
	var socket = io.connect();

	socket.on('message', function (data) {	// when message in from server
		var output = '<li>'
						+ data.date + ' [' + data.name + '] - ' + data.message
					+ '</li>';
		
		$(output).prependTo('#chat_content');
	});
	
	socket.on('users_count', function (data) {	// when change number of user in page
		var chatHeading = '대화';
		$('#chat').text(chatHeading + '(' + data.number + ')');
		if(typeof data.id !="undefined") $('#name').val(data.id.substring(0, 9));
	});
	
	socket.on('refresh_connection', function (data) {
		$('#user_section').empty().append($(data));
	});
	
	// event /////////////////////////////////////////////////////
	
	$('#send').click(function() {
		// WTS Message
		$('input.session_user:checked').each(function() {
			var sid = $(this).attr('id');
			var param = {to: sid, msg: $('#message').val()};
			$.post('/message', param);
			$(this).attr('checked', false);	// uncheck
			$list = $(this).parent();
			if( 0 == $list.find('.disconn_user').length ) {
				$list.append($('<button class="disconn_user">X</button>'));
			}
		});
		// Chat Message
		var msg_data = {
				name: $('#name').val(), message: $('#message').val(),
				date: getCurrentTime() 	};
		if ( $.browser.msie && parseInt($.browser.version, 10) < 8) {
			$.post('chat', msg_data);
		} else {
			socket.emit('message', msg_data);
		}
		$('#message').select().focus();
	});
	
	$('#message').keydown(function(event) {
		if (event.which == 13) {
			event.preventDefault();	// IE enter event try to submit
			$('#send').click();
		}
	}).click(function() {
		$(this).select();
	}).focus();
	
	$('#name').click(function() {
		$(this).select();
	});
	
	$(document).delegate('button.disconn_user', 'click', null, (function() {
		var sid = $(this).parent().find('.session_user').attr('id');
		$.post('/disconnect/'+sid);
	}));
});