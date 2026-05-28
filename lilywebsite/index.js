document.addEventListener("DOMContentLoaded", function() {
    var images = document.querySelectorAll('img');
    var navLinks = document.querySelectorAll('nav a');

    var colors = ['#f8d3fe', '#98e6cd', '#adc8fe', '#fee2a1', '#cebefe']; // Array of 5 different color choices

    images.forEach(function(image) {
        var randomColorIndex = Math.floor(Math.random() * colors.length); // Generate a random index to pick a color
        var randomColor = colors[randomColorIndex]; // Get the random color from the array
        image.style.border = '5px solid ' + randomColor; // Apply the random color as the border
    });

    for (var i = colors.length - 1; i > 0; i--) {
        var j = Math.floor(Math.random() * (i + 1));
        [colors[i], colors[j]] = [colors[j], colors[i]];
    }

    // Assign shuffled colors to navigation tabs
    navLinks.forEach(function(link, index) {
        var colorIndex = index % colors.length; // Use modulus operator to cycle through colors array
        var randomColor = colors[colorIndex]; // Get the color from the shuffled array
        link.style.backgroundColor = randomColor; // Apply the color as the background color
        link.style.color = '#000000'; // Set the text color to black
    });
});

document.addEventListener("DOMContentLoaded", function () {
        const grid = document.querySelector(".photo-grid");
        const images = Array.from(grid.children);

        // Shuffle the images
        images.sort(() => Math.random() - 0.5);

        // Re-append images to the grid in the new order
        images.forEach((img) => grid.appendChild(img));
    });
