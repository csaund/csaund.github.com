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

function initialize() {
    var mapCanvas = document.getElementById('world-map');
	console.log(mapCanvas);
    var mapOptions = {
      center: new google.maps.LatLng(44.5403, -78.5463),
      zoom: 8,
      mapTypeId: google.maps.MapTypeId.ROADMAP
    }
    var map = new google.maps.Map(mapCanvas, mapOptions);
}

