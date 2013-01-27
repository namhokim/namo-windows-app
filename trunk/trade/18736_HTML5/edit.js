// jQuery
$( document ).ready(function() {
	var canvas = $('#layer');
	var canvasWidth = canvas.width();
	var canvasHeight = canvas.height();
	
	var context = canvas.get(0).getContext("2d");
	var textInput = $('#myText');
	var textObjects = new Array();	// 텍스트객체를 저장할 배열
	var selectRect = null;					// 선택영역 표시
	var refreshRepeat = false;	// 화면갱싱을 반복할지 플래그
	
	/* 텍스트 객체 */
	var Text = function(text, x, y, size, font, color) {
		this.text = text;
		this.x = x;
		this.y = y;
		this.size = size;
		this.font = font;
		this.color = color;
	};
	
	/*   캔버스 내 텍스트창에 있는 글자 쓰기 함수 <= function draw()   */
	$('#btn_text_submit').click(function() {
		var text = textInput.val();
		var size = $('#fontSize').val();
		var font = $('#fontFace option:selected').val();
		var color = $('#fontColor option:selected').val();
		textObjects.push(new Text(text, 40, 40, size, font, color));	// 추가
		textInput.val('');	// 입력컨트롤 값 초기화
		refresh();			// 화면 갱신
	});
	
	/* 캔버스 클릭시 */
	canvas.click(function() {
		alert('cl can');
	});
	
	/* 객체들을 화면에 그려주는 함수 */
	function refresh() {
		// 화면 초기화
		context.clearRect(0,0,canvasWidth, canvasHeight);
		
		// 텍스트 처리
		var objLen = textObjects.length;
		for (var i=0; i<objLen; i++) {
			var tObj = textObjects[i];

			context.font = tObj.size + 'px ' + tObj.font;
			context.fillStyle = tObj.color;
			context.fillText(tObj.text, tObj.x, tObj.y);
		}
		
		// 이미지처리
		
		// 선택처리
		if (selectRect!=null) {
			alert('aa');
		}
		
		if (refreshRepeat) {	// 애니메이션을 위한 반복시
			setTimeout(refresh, 33);
		}
	};
	
});