// To do:
//      - Preserve aspect ratio of videos across window resizing
//      - Eliminate nav-bar 'flicker' when navigating across pages in 
//          narrow-window mode

$(document).ready(highlight_nav);
$(document).on("scroll", highlight_nav);

$(document).ready(check_navbars);
$(window).resize(check_navbars);

function highlight_nav() {
    const screenTop = $(document).scrollTop();
    const screenBottom = screenTop + $(window).height();

    $('#nav_bar_2 a').each( function () {
        const currLink = $(this);
        const refElement = $(currLink.attr("href"));


        if ( refElement.length == 0 ) {
            // Link exists in nav but corresponding div does not exist
            ;
        } else {

            var elemTop = refElement.position().top;
            var elemBottom = elemTop + refElement.height();

            if ( elemTop >= screenTop && elemTop + 100 < screenBottom ) {
                // Can see top
                currLink.addClass("active");
            } else if ( elemTop < screenTop && elemBottom > screenBottom ) {
                // Are in the middle
                currLink.addClass("active");
            } else if ( elemBottom - 10 > screenTop && elemBottom < screenBottom ) {
                // Can see bottom
                currLink.addClass("active");
            } else {
                // Otherwise
                currLink.removeClass("active");
            }
        }
    });
}

// Hides nav_bar_2
function hide_nav() {
    $("#nav_bar_2").hide();
    $(".content").each(function() {
        $(this).addClass("content-wide");
    });
    $("#nav_home").css('width', '120pt');
}


// Shows nav_bar_2
function show_nav() {
    $("#nav_bar_2").show();
    $(".content").each(function() {
        $(this).removeClass("content-wide");
    });
    $("#nav_home").css('width', '240pt');
}

function check_navbars() {
    if( window.innerWidth < 1150 ) {
        hide_nav();
    } else {
        show_nav();
    }
}
