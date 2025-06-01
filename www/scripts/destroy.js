const btn = document.getElementById("destroy-btn");

btn.addEventListener("click", destroyEverything);

function destroyEverything() {
	const message = createMessage();
	const web = document.querySelector("body");
	const gif = createGif();

	message.innerText = "A tomar por culo la web";
	web.appendChild(gif);
	setTimeout(() => {
		while (web.firstChild)
			web.removeChild(web.firstChild);
		web.appendChild(message);
	}, 2000)
}

function createGif() {
	const gif = document.createElement("img");

	gif.src = "https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExZmF0bHBqemlhMzBldmd5YTJwempmOXhqaDV5Zm93YW0zY25qZ2o2dSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/XUFPGrX5Zis6Y/giphy.gif";
	gif.style.position = "fixed";
	gif.style.top = "0";
	gif.style.left = "0";
	gif.style.width = "100vw";
	gif.style.height = "100vh";
	gif.style.objectFit = "cover";
	gif.style.zIndex = "9999";
	return (gif);
}

function createMessage() {
	const message = document.createElement("h1");

	message.style.textAlign = "center";
	message.style.fontSize = "3rem";
	return (message);
}