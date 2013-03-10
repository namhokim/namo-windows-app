/* jQuery - DOM hierarchy has been fully constructed */
$( document ).ready(function() {

	/* ready내 전역변수들 */
	var URL = window.webkitURL || window.URL;
	var can, ctx, canX, canY, mouseIsDown, initX, initY, selX, selY;
	var canvas, context, canvasWidth, canvasHeight;
	var textObjects, imageObjects, refreshRepeat, selectedObj;
	var textInput, fontSize, fontFace, fontColor, bgColor;
	var textSubmitButton, textClearButton;
	var backgroundColor;
	var socket;
	
	/* 초기화 함수 호출 */
	init();

	//////////////////////////////////////////////////////////////////////
	
	/* 초기화 */
	function init() {
		// native javascript
		can = document.getElementById("layer");
		ctx = can.getContext("2d");
		mouseIsDown = false;
		initInitPosition();
		
		// jQuery
		canvas = $('#layer');
		context = canvas.get(0).getContext("2d");
		canvasWidth = canvas.width();
		canvasHeight = canvas.height();
		
		textObjects = new Array();	// 텍스트객체를 저장할 배열
		imageObjects = new Array();	// 이미지객체를 저장할 배열
		refreshRepeat = false;	// 화면갱신을 반복할지 여부
		selectedObj = null;         // 선택 객체(미선택시 null)
		
		// elements
		textInput = $('#myText');
		fontSize = $('#fontSize');
		fontFace = $('#fontFace');
		fontColor = $('#fontColor');
		bgColor = $('#bgColor');
		textSubmitButton = $('#btn_text_submit');
		textClearButton = $('#btn_text_clear');
		
		backgroundColor = "white";
		refresh();
		
		// event handlers
		can.addEventListener("mousedown", mouseDown, false);
		can.addEventListener("mousemove", mouseXY, false);
		can.addEventListener("touchstart", touchDown, false);
		can.addEventListener("touchmove", touchXY, true);
		can.addEventListener("touchend", touchUp, false);
		document.body.addEventListener("mouseup", mouseUp, false);
		document.body.addEventListener("touchcancel", touchUp, false);
		
		/* socket.io 부분(서버로 전송을 위해) */
		if( typeof sock_io_addr != "undefined") {
			//alert(sock_io_addr);
			socket = io.connect(sock_io_addr);
		} else {
			socket = null
		}
	}
	
	/* 이벤트좌표 초기화 */
	function initInitPosition()
	{
		initX = NOT_SELECTED;
		initY = NOT_SELECTED;
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
	
	/*   캔버스 내 텍스트창에 있는 글자 쓰기 함수 <= function draw()   */
	textSubmitButton.click(function() {
		var text = textInput.val();
		if (text.length == 0) return;	// 입력글자의 수가 zero이면 submit 불가
		var size = fontSize.val();
		var font = fontFace.val();
		var color = fontColor.val();
		textObjects.push(new TextObj(text, 0, canvasHeight, size, font, color));	// 추가
		textInput.val('');	// 입력컨트롤 값 초기화
		refreshIfNotRepeat();			// 화면 갱신
	});
	
	/* 텍스트 선택 삭제 */
	textClearButton.click(function() {
		if (selectedObj!=null) {
			// 텍스트 처리
			var objLen = textObjects.length;
			for (var i=0; i<objLen; i++) {
				if (selectedObj === textObjects[i]) {
					textObjects.splice(i, 1);
					setTextHandler(selectedObj, false);
					selectedObj = null;
					break;
				}
			}
			refresh();
		}
	});

	/* 이미지 선택시 이벤트 */
	$('#imageSrc').change(function(e) {
		selImage = URL.createObjectURL(e.target.files[0]);
	});
	/* 이미지 추가 이벤트 */
	$('#imageAdd').click(function() {
		if (selImage == null) return;

		var img = new Image();
		img.onload = function() {imageObjects
			var image = new ImageObj(this, 0, 0);
			imageObjects.push(image);
			refreshIfNotRepeat();			// 화면 갱신
		}
		img.src = selImage;
		selImage = null;
	});
	
	/* 배경색 변경시 이벤트 */
	bgColor.change(function(e) {
		backgroundColor = bgColor.val();
		refresh();
	});
	
	/* 서버로 전송 */
	$('#send').click(function() {
		var data = {bgColor : backgroundColor, texts : textObjects, refreshAgain : refreshRepeat};
		socket.emit('data', {draw: data});
	});

	/* 모션관련 컨트롤 이벤트 */
	$('#motion-ud').click(function() {
		if (selectedObj==null) return;
		selectedObj.motionType = MOTION_TYPE_VERTICAL;
		refreshAndRepeatIfNotRepeat();
	});
	$('#motion-lf').click(function() {
		if (selectedObj==null) return;
		selectedObj.motionType = MOTION_TYPE_HORIZON;
		refreshAndRepeatIfNotRepeat();
	});
	
	$('#complete').click(function() {
		$(location).attr('href', './antechamber');
	});
	
	//////////////////////////////////////////////////////////////////////
	/* 마우스 클릭시 */
	function mouseDown() {
		mouseIsDown = true;
		mouseXY();
	}
	function mouseUp() {
		mouseIsDown = false;
		mouseXY();
		initInitPosition();
	}
	function mouseXY(e) {
		handleXY(true);
	}

	/* 터치시 */
	function touchDown() {
		mouseIsDown = true;
		touchXY();
	}
	function touchUp() {
		mouseIsDown = false;
		
		// 이벤트 종료(touch)
		if(isSamePostion(initX, initY, canX, canY)) {
			if (selectedObj!=null) {
				getAttributeToControl();
			}
		} else {
			unselect(true);
		}
		initInitPosition();
	}

	function touchXY(e) {
		handleXY(false);
	}

	function handleXY(isMouse, e) {
		if (!e) var e = event;
		e.preventDefault();
		if (isMouse) {
			canX = e.pageX - can.offsetLeft;
			canY = e.pageY - can.offsetTop;
		} else {
			canX = e.targetTouches[0].pageX - can.offsetLeft;
			canY = e.targetTouches[0].pageY - can.offsetTop;
		}
		
		if (mouseIsDown) {
			if (isBeforeUp()) {
				// 이벤트 시작(mouse/touch)
				initX = canX;
				initY = canY;
				selectHandler(canX, canY);	// 선택
			} else {
				// 이동
				if(selectedObj!=null) {
					selectedObj.x = selX + (canX-initX);
					selectedObj.y = selY + (canY-initY);
					refreshIfNotRepeat();
				}
			}
		} else {
			if (!isBeforeUp()) {
				// 이벤트 종료(mouse)
				if(isSamePostion(initX, initY, canX, canY)) {
					if (selectedObj!=null) {
						getAttributeToControl();
					}
				} else {
					unselect(true);
				}
			}
		}
		
	}
	
	// 개발용 테스트용
	function showPos() {
		var str = canX + ", " + canY;
		if (mouseIsDown) {
			str += " down";
		} else {
			str += " up";
		}
		textInput.val(str);
	}
	
	function isBeforeUp() {
		return (initX == NOT_SELECTED);
	}
	
	function isSamePostion(x1, y1, x2, y2) {
		return ( (x1==x2) && (y1==y2) );
	}
	
	/* 해당 좌표에 텍스트 객체가 있으면 선택, 없으면 해제한다. */
	function selectHandler(x, y) {
		var newSelectedObj = getCanvasObject(x, y);
		if (newSelectedObj==null) {
			unselect(false);
		} else {
			if(selectedObj==null) {
				selectStatus(newSelectedObj);
			} else {
				if (selectedObj===newSelectedObj) {
					unselect(false);
				} else {
					selectStatus(newSelectedObj);
				}
			}
		}
		refreshIfNotRepeat();
	}
	
	function selectStatus (newSelectedObject) {
		selectedObj = newSelectedObject;
		selX = selectedObj.x;
		selY = selectedObj.y;
	}
	
	function unselect(needRefresh) {
		selectedObj = null;	// 선택해제
		setTextHandler(null, false);
		if (needRefresh) {
			refreshIfNotRepeat();
		}
	}

	//////////////////////////////////////////////////////////////////////
	
	/* 애니메이션 효과가 아닐때 refresh 수행 */
	function refreshIfNotRepeat() {
		if (!refreshRepeat)
		{
			refresh();
		}
	}

	/* 애니메이션 효과가 아닐때 애니메이션 효과 지정+refresh 시작 */
	function refreshAndRepeatIfNotRepeat() {
		if (!refreshRepeat)
		{
			refreshRepeat = true;
			refresh();
		}
	}

	/* 객체들을 화면에 그려주는 함수 */
	function refresh() {
		// 화면 초기화
		context.clearRect(0,0,canvasWidth, canvasHeight);
		
		// 배경색
		context.fillStyle = backgroundColor;
		context.fillRect(0,0,canvasWidth, canvasHeight);
		
		// 이미지처리
		objLen = imageObjects.length;
		for (var i=0; i<objLen; i++)
		{
			var tObj = imageObjects[i];
			context.drawImage(tObj.image, tObj.x, tObj.y);
		}

		// 텍스트 처리
		var objLen = textObjects.length;
		for (var i=0; i<objLen; i++) {
			var tObj = textObjects[i];

			context.font = tObj.makeFontString();
			context.fillStyle = tObj.color;
			context.fillText(tObj.text, tObj.x, tObj.y);
			processMotion(tObj, canvasWidth, canvasHeight);
		}
		
		// 선택처리
		if (selectedObj!=null) {
			var rect = TextToRectangle(selectedObj);
			context.strokeStyle="#FF0000";
			context.strokeRect(rect.x, rect.y, rect.width, rect.height);
		}
		
		if (refreshRepeat) {	// 애니메이션을 위한 반복시
			setTimeout(refresh, 33);
		}
	};

	//////////////////////////////////////////////////////////////////////
	
	/* 해당 포인터에 텍스트객체가 있으면 반환, 없으면 null 반환 */
	function getCanvasObject(x, y) {
		var objLen = textObjects.length;

		for (var i=0; i<objLen; i++) {
			var tObj = textObjects[i];
			if (containTextObject(tObj, x, y)) {
				return tObj;	// 배열 앞의 제일 처음만 선택
			}
		}

		return null;	// 찾지 못했다
	}

	/* 텍스트객체에 해당 포인터가 속해있는지 판단 */
	function containTextObject(textObject, x, y) {
		var rect = TextToRectangle(textObject);
		
		var minX = rect.x;
		var minY = rect.y;
		var maxX = rect.x + rect.width;
		var maxY = rect.y + rect.height;
		
		return ( (minX<=x) && (x<=maxX) && (minY<=y) && (y<=maxY) );
	}
	
	/* 텍스트객체를 직사각형 객체로 변환 */
	function TextToRectangle(textObject) {
		if (textObject==null) throw new "textObject parameter cannot be null";
		
		var x = textObject.x;
		var y = textObject.y - textObject.size;	// text객체는 좌측아래를 기준점으로 사용
		context.font = textObject.makeFontString();
		var width = context.measureText(textObject.text).width;
		var height = textObject.size;
		
		return new Rectangle(x, y, width, height);
	}

	
	/* 캔버스에서 선택된 텍스트객체의 속성을 컨트롤에 반영 */
	function getAttributeToControl()
	{
		if (selectedObj==null) return;

		textInput.val(selectedObj.text);
		fontSize.val(selectedObj.size).slider('refresh');
		fontFace.val(selectedObj.font).selectmenu('refresh');
		fontColor.val(selectedObj.color).selectmenu('refresh');
		
		// 핸들러 연결
		setTextHandler(selectedObj, true);
	}
	
	/* 텍스트 컨트롤과 텍스트객체 연결/해제 */
	function setTextHandler(textObject, isConnect) {
		if(isConnect) {
			if (selectedObj==null) return;
			
			textInput.on('keyup', function(e) {
				selectedObj.text = $(this).val();
				refreshIfNotRepeat();
			});
			
			fontSize.on('change', function(e) {
				selectedObj.size = Number($(this).val());
				refreshIfNotRepeat();
			});
			
			fontFace.on('change', function(e) {
				selectedObj.font = $(this).val();
				refreshIfNotRepeat();
			});
			
			fontColor.on('change', function(e) {
				selectedObj.color = $(this).val();
				refreshIfNotRepeat();
			});
		} else {
			textInput.val("");
			textInput.off('keyup');
			fontSize.off('change');
			fontFace.off('change');
			fontColor.off('change');
		}
	}
	
});