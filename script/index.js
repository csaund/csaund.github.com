$(document).ready(function(){ 
	$(".my").addClass("hidden");
	$(".who").removeClass("hidden");

	$("a").on("click",function(){
		$(".my").addClass("hidden");
		var cl = $(this).attr('class');
		cl = '.' + cl;
		$(cl).removeClass("hidden");
			
	});

});


