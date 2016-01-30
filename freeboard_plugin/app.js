$.support.cors = true;

setTimeout(function(){	
	$("[type='checkbox']").bootstrapSwitch();
},3000);

window.setRL78Text = function(text) {
	if (window.targetThing == "") {
		freeboard.showDialog($("<div align='center'>Error: Please set thing name!</div>"),"Error!","OK",null,function(){}); 	
		return;	
	}
	dweetio.dweet_for(window.targetThing+'-send', {"lcd_text4":text}, function(err, dweet){ if(!err) $('button#lcdsubmit').prop("disabled",false);});
 	freeboard.showLoadingIndicator(true);
		setTimeout(function(){	
			freeboard.showLoadingIndicator(false);
			freeboard.showDialog($("<div align='center'>Text sent to RL78 "+window.targetThing+" LCD!</div>"),"Success!","OK",null,function(){}); 
		},750);	
}

window.sendBuzz = function() {
	if (window.targetThing == "") {
		freeboard.showDialog($("<div align='center'>Error: Please set thing name!</div>"),"Error!","OK",null,function(){}); 	
		return;	
	}
	dweetio.dweet_for(window.targetThing+'-send', {"beep":true}, function(err, dweet){});
 	freeboard.showLoadingIndicator(true);
	setTimeout(function(){	
		freeboard.showLoadingIndicator(false);
		freeboard.showDialog($("<div align='center'>Buzzer activated on RL78 "+window.targetThing+"</div>"),"Success!","OK",null,function(){}); 
	},750);	
}

