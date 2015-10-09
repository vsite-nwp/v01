#### vježba 1

Analizirajte minimalni Win32 program sa prvog predavanja.

Promijenite pozadinu glavnog prozora na boju *cyan*. Koristite funkciju [CreateSolidBrush](https://msdn.microsoft.com/en-us/library/dd183518%28v=vs.85%29.aspx).

Na glavni prozor dodajte dva prozora tipa "button". Koristite funkciju [CreateWindow](https://msdn.microsoft.com/en-us/library/ms632679%28VS.85%29.aspx) sa stilom `WS_CHILD|WS_VISIBLE`.

![main window](/screenshot.png?raw=true)

Klik na svaki *button* neka pokazuje *message box* - sa različitim tekstovima. Koristite funkciju [MessageBox](https://msdn.microsoft.com/en-us/library/windows/desktop/ms645505%28v=vs.85%29.aspx) sa ikonom upozorenja.

![message box](/msgbox.png?raw=true)

