// Sélection de la balise <section> où nous allons afficher les heures
const settimeSection = document.getElementById("settime");

const hibernationSelect = document.querySelector('select[name="hibernation"]');
const reveilSelect = document.querySelector('select[name="reveil"]');

// Fonction pour effectuer la requête
function fetchTime() {
	fetch('/gettime')
		.then(response => response.text())
		.then(data => {
			// Extrait les heures de départ et de fin de la chaîne de données
			
			const startTime = data.slice(0, 2)
			const endTime = data.slice(2, 4)
			
			hibernationSelect.value = startTime + 'h';
			reveilSelect.value = endTime + 'h';

			console.log(data);

			// Construction du contenu HTML à afficher
			const content = `
				<p>Heure d'hibernation : ${startTime}:00</p>
				<p>Heure de réveil : ${endTime}:00</p>
      			`;


			// Attribution du contenu à la balise <section>
			settimeSection.innerHTML = content;
		})
		.catch(error => {
			console.error('Une erreur s\'est produite:', error);
		});
}

// Appel de la fonction pour récupérer et afficher les heures
fetchTime();
