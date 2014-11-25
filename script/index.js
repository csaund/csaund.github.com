$(document).ready(function(){ 
	$(".my").addClass("hidden");
	$(".who").removeClass("hidden");


	//trolololo I'm so sorry I should
	//have overwritten the default instead
	//of made <a> elements but I was deciding
	//whether to make a bunch of html files
	//or do show/hide. this was so much easier.
	$("a").on("click",function(){
		$(".my").addClass("hidden");
		var cl = $(this).attr('class');
		cl = '.' + cl;
		$(cl).removeClass("hidden");
	});
});


