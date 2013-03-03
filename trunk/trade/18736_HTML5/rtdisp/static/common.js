// JavaScript Document

/* 상수 */
var NOT_SELECTED = Number.MAX_VALUE;
var MOTION_TYPE_NONE = 1;		// 모션미사용
var MOTION_TYPE_VERTICAL = 2;	// 상/하
var MOTION_TYPE_HORIZON = 3;	// 좌/우

//var sock_io_addr = "http://222.237.65.198:52273";
var sock_io_addr = "http://localhost:52273";
var refreshAgain = false;

/* 객체들을 화면에 그려주는 함수 */
function refreshCanvas(canvas, data) {

	// 공통 변수
	var canvasWidth = canvas.width();
	var canvasHeight = canvas.height();
	var context = canvas.get(0).getContext("2d");
	refreshAgain = data.draw.refreshAgain;
	
	// 화면 초기화
	context.clearRect(0,0,canvasWidth, canvasHeight);
	
	// 배경색
	context.fillStyle = data.draw.bgColor;
	context.fillRect(0,0,canvasWidth, canvasHeight);
	
	// 텍스트 처리
	var textObjects = data.draw.texts;
	var objLen = textObjects.length;
	for (var i=0; i<objLen; i++) {
		var tObj = textObjects[i];

		context.font = makeFontString(tObj.size, tObj.font);
		context.fillStyle = tObj.color;
		context.fillText(tObj.text, tObj.x, tObj.y);
		processMotion(tObj,canvasWidth, canvasHeight);
	}
	
	//if (refreshAgain) {	// 애니메이션을 위한 반복시
	//	setTimeout(function() {
	//		refreshCanvas(canvas, data);
	//	}, 33);
	//}
};

/* Canvas context에 글꼴을 적용하기 위한 문자열 생성함수*/
function makeFontString(size, font) {
	return size + 'px ' + font;
}

/* 모션 처리 */
function processMotion(textObject, canvasWidth, canvasHeight) {
	var VELOCITY = 2;	// 2 px/frame

	switch (textObject.motionType) {
	case MOTION_TYPE_VERTICAL:
		var ty = textObject.y;
		if (textObject.motionToPositive) {
			textObject.y += VELOCITY;
			if (textObject.y > canvasHeight)
			{
				textObject.y = canvasHeight;
				invertDirection(textObject);
			}
		} else {
			textObject.y -= VELOCITY;
			if ((textObject.y - textObject.size) < 0)
			{
				textObject.y = textObject.size;	// 기준이 아래임
				invertDirection(textObject);
			}
		}
		break;
	case MOTION_TYPE_HORIZON:
		var rect = TextToRectangle(textObject);

		if (textObject.motionToPositive) {
			if (rect.width >= canvasWidth) {	// 빠르게 튀김 방지
				textObject.x = (canvasWidth - rect.width);
				break;
			}

			textObject.x += VELOCITY;
			
			if ((rect.x+rect.width) > canvasWidth)
			{
				textObject.x = (canvasWidth - rect.width);
				invertDirection(textObject);
			}
		} else {
			textObject.x -= VELOCITY;
			if (textObject.x < 0)
			{
				textObject.x = 0;	// 기준이 왼쪽임
				invertDirection(textObject);
			}
		}
		break;
	}
}

/* 방향 전환 함수 : 캔버스 경계를 초과했을 경우 호출 */
function invertDirection(textObject) {
	if (textObject.motionToPositive) {
		textObject.motionToPositive = false;
	} else {
		textObject.motionToPositive = true;
	}
}