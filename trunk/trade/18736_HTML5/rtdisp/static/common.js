// JavaScript Document

/* 상수 */
var NOT_SELECTED = Number.MAX_VALUE;
var MOTION_TYPE_NONE = 1;		// 모션미사용
var MOTION_TYPE_VERTICAL = 2;	// 상/하
var MOTION_TYPE_HORIZON = 3;	// 좌/우

//var sock_io_addr = "http://222.237.65.198:52273";
var sock_io_addr = "http://localhost:52273";

// 공통 변수
var canvasWidth;
var canvasHeight;
var context;
var refreshAgain = false;
var refreshing = false;
var backgroundColor = "white";
var textObjs = new Array();	// 텍스트객체를 저장할 배열

/* 초기화 */
function initCommon(canvas) {
	canvasWidth = canvas.width();
	canvasHeight = canvas.height();
	context = canvas.get(0).getContext("2d");
}

/* 데이터 갱신 */
function refreshData(data) {
	// 다시 그릴 여부 설정
	refreshAgain = data.draw.refreshAgain;
	if (!refreshAgain) refreshing = false;
	
	// 배경색
	backgroundColor = data.draw.bgColor;
	
	// 텍스트 데이터
	var objLen = data.draw.texts.length;
	textObjs = new Array();
	for (var i=0; i<objLen; i++) {
	  var aText = data.draw.texts[i];
	  var nText = new TextObj(aText.text, aText.x, aText.y, aText.size, aText.font, aText.color);
	  nText.motionType = aText.motionType;
	  nText.motionToPositive = aText.motionToPositive;

	  textObjs.push(nText);	// 추가
	}
	
	// 화면 초기화
	refreshGuard();
}

function refreshGuard() {
	if (refreshing) return;
	
	if (refreshAgain) refreshing = true;
	refreshCanvas();
}

/* 객체들을 화면에 그려주는 함수 */
function refreshCanvas() {
	// 화면 초기화
	context.clearRect(0,0,canvasWidth, canvasHeight);
	
	// 배경색
	context.fillStyle = backgroundColor;
	context.fillRect(0,0,canvasWidth, canvasHeight);
	
	// 텍스트 처리
	var objLen = textObjs.length;
	for (var i=0; i<objLen; i++) {
		var tObj = textObjs[i];

		context.font = tObj.makeFontString();
		context.fillStyle = tObj.color;
		context.fillText(tObj.text, tObj.x, tObj.y);
		processMotion(tObj,canvasWidth, canvasHeight);
	}
	
	if (refreshAgain) {	// 애니메이션을 위한 반복시
		setTimeout(function() {
			refreshCanvas();
		}, 33);
	}
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
		var rect = textObject.textToRectangle();

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

	//////////////////////////////////////////////////////////////////////
	/* 텍스트 객체 */
	var TextObj = function(text, x, y, size, font, color) {
		this.text = text;
		this.x = Number(x);
		this.y = Number(y);
		this.size = Number(size);
		this.font = font;
		this.color = color;
		this.motionType = MOTION_TYPE_NONE;
		this.motionToPositive = true;
	};

  TextObj.prototype.makeFontString = function()
  {
    return this.size + 'px ' + this.font;
  };

  TextObj.prototype.textToRectangle = function()
  {
    var x = this.x;
		var y = this.y - this.size;	// text객체는 좌측아래를 기준점으로 사용
		context.font = this.makeFontString();
		var width = context.measureText(this.text).width;
		var height = this.size;
		
		return new Rectangle(x, y, width, height);
  };

  //////////////////////////////////////////////////////////////////////
  
  	/* 이미지 객체 */
	var ImageObj = function(image, x, y) {
		this.image = image;
		this.x = Number(x);
		this.y = Number(y);
	};
	
	/* 직사각형 객체 */
	var Rectangle = function(x, y, width, height) {
		this.x = Number(x);
		this.y = Number(y);
		this.width = Number(width);
		this.height = Number(height);
	};
	
	/* 선택 객체 */
	var SelectRect = function(x, y, width, height) {
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
	};

	//////////////////////////////////////////////////////////////////////