// jQuery
$( document ).ready(function() {
	var canvas = $('#layer');
	var context = canvas.get(0).getContext("2d");
	
	/*   캔버스 내 텍스트창에 있는 글자 쓰기 함수 <= function draw()   */
	$('#btn_text_submit').click(function() {
		var text = $('#myText').val();
		var size = $('#fontSize').val();
		var font = $('#fontFace option:selected').val();
		var color = $('#fontColor option:selected').val();
		//var textLen = (text.length * fontSize)/2;
		//alert(cont_font);
		context.clearRect(0,0,canvas.width(), canvas.height());
		context.font = size + 'px ' + font;
		context.fillStyle = color;
		context.fillText(text, 40,40);
		context.beginPath();
		context.closePath();
		context.stroke();
	});
});