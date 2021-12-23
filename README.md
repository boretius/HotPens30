# HotPens
 <h3><br />
  <strong>Duale  DIY-Lötstation auf Arduino-Basis für Weller Lötspitzen, mit Touchbedienung und  Drehregler.</strong></h3>
<p><br />
  <img src=https://user-images.githubusercontent.com/35202339/145238639-1c9fe5e5-7e9c-4fde-a94c-39a3fb422904.jpg width=60% height=Auto alt="Hotpens Lötstation" /><br />
  Von Weller gibt es für den professionellen Bereich zwei Lötkolben Serien (  40Watt SMD, 150Watt Standard), die sich durch ihre aktiven Lötspitzen von  anderen Lötstationen abheben. Für um 40€ (pro Lötspitze) bekommt man eine große  Auswahl an Lötspitzen, die sich durch eine außergewöhnlich kurze Aufheizzeit (im  einstelligen Sekundenbereich) und einer direkt in der Spitze integrierten  Temperaturmessung auszeichnen. Nur schade, dass Weller für eine Duallötstation  um die 1.000,- € haben will. Wir haben hier eine Alternative für Maker, die mit  Touchbedienung und Drehregler nicht viel Wünsche offen lässt. </p>
<ul type="disc">
  <li>6  Temperatur-Presets (3 Pro       Kanal)</li>
  <li>Pro Kanal können 4 Lötspitzen Kalibrierungen gespeichert werden</li>
  <li>Temperaturbereich von 200-450°C</li>
  <li>Touch-Farbdisplay </li>
  <li>Einfache Temperaturvorwahl durch Drehregler</li>
  <li>Auto-Standby </li>
  <li>Auto-Wakeup durch Berühren eines Lötschwamms</li>
  <li>Einfache Lötspitzentemperatur-Kalibrierung </li>
  <li>Optisches Feedback der Heizleistung durch LED</li>
  <li>Erweiterung zu Labornetzteil (in Planung)</li>
  <li>Open-Source-Software auf Basis von Arduino</li>
</ul>
<h3>&nbsp;</h3>
<h3><strong>Touch-Display</strong><br />
  <img src=https://user-images.githubusercontent.com/35202339/145238618-af8ae4f7-e108-496d-89af-1ec7827db5ff.jpg width=50% height=Auto alt="Menu" /><strong> </strong><br />
</h3>
<h3>&nbsp;</h3>
<h3><strong>Bedienung</strong></h3>
<p>  Auf dem Display sind die beiden Lötkolbenbereiche vertikal in je 2 Spalten  getrennt dargestellt. Die äußeren Spalten beinhalten oben den Aktivierungs-Button  und darunter die Temp-Presets. Die inneren Spalten zeigen von oben: die aktuelle  Lötspitzentemperatur, die Vorwahltemp. bzw. Standbybutton, darunter das Feld  für Fehlermeldungen und unten die Menu-Bedienung. Der Druck auf die jeweils ind den oberen Ecken liegenden Buttons startet bzw. endet die Aufheizung., wobei die grüne Farbe den Aufheizvorgang anzeigt. Nach vier Minuten Inaktivität schaltet die Heizung zurück auf &quot;StandBy&quot;, was dann mit &quot;gelb&quot; dargestellt wird. Rot bedeutet &quot;Aus&quot;. Die Anzeite &quot;Tx&quot; weist auf die gewählte Lötspitze mit den entsprechenden Arbeitsparametern hin, die während eines Kalibriervorganges im Speicher hinterlegt wurden. Ein Touch auf  einen Temperaturwahl-Button   aktiviert die Einstellmöglichkeit mittels des Drehreglers. Die so eingestellten Werte speichert das Bedienprogramm direkt ab.<br />
</p>
<p>&nbsp;</p>
<h3><strong>Kalibriermenu</strong><br />
  <strong> <img src=https://user-images.githubusercontent.com/35202339/145238652-fbaf2e2b-d266-4921-92cf-eb8d153079f1.JPG width=25% height=Auto alt="Menu2" /></strong> <img src=https://user-images.githubusercontent.com/35202339/145238663-fd4d3454-1075-425a-8807-d81a5761e920.JPG width=25% height=Auto alt="Kalibierung" /><br />
</h3>
<h3>&nbsp;</h3>
<h3>Kalibrierung</h3>
Im Kalibriermenü können bis zu 8 unterschiedliche Lötspitzentypen kalibriert  und gespeichert werden. Die Kalibrierung ist menugeführt. Für jeden Lötspitzentyp lassen sich die untere Schwelltemperatur (&quot;kalt&quot;) und eine wählbare obere Temeratur (&quot;<em>250</em>&quot;) kalibrieren. Hierzu wird ein geeignetes  Thermometer benötigt. Alternativ kann man ein Lötzinn mit bekanntem  Schmelzpunkt verwenden.<br />

<h3>&nbsp;</h3>
<h3>Bauanleitung</h3>
Die Bauanleitung steht unter HotPensDoku.zip (siehe oben) zum Download bereit.<br />

<h3>&nbsp;</h3>
<h3>Platine</h3>
Platinenwünsche nimmt der Autor unter der Kontaktadresse bei www.netb.de gerne entgegen.<br />

<h2>&nbsp;</h2>
<h2><strong>Eweiterungen</strong></h2>
<h3><br />
  <strong>Ablage-Sensor </strong></h3>
<p><br />
  Es ist auch möglich den Standby durch einen Ablagesensor auszulösen. Dazu  wird allerdings ein Lötkolbenständer mit Schalter benötigt. Dann wird mit der  Ablage automatisch der Standbymodus aktiv und beim Entnehmen des Lötkolbens  heizt er sofort wieder auch die Vorwahltemperatur aus ( 1 bis 2 Sekunden)<br />
  </p>
<h3>&nbsp;</h3>
<h3><strong>Festspanungsnetzteil</strong></h3>
<p>  Noch in Planung<br />
  </p>
<h3><strong>Labornetzteil</strong></h3>
<p>  Noch in Planung</p>

<br>
23. Dezember 2021
<br>
Idee und Motivation: Hubert Schmucker
<br>
Ausführung: Michael Boretius
