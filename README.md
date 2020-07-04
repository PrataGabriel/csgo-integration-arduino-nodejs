# Integração do arduino com nodejs no Counter-Strike Global Offensive utlizando leds RGB 5050

Passo 1:
	Inicialize o código do arduino (arduino/PcLeds.ino) no IDE e configure os pinos digitais de acordo com a sua preferência.

Obs.: Foi utilizado os pinos na respectivas cores: Red = 5, Green = 3, Blue = 6.
	  Utilizei 6 pinos pois utilizava 2 fitas de leds em conexões distintas.

Passo 2:
	Ao finalizar a configuração do seu projeto Arduino, carregue-o no arduino de sua escolha (não esqueça das configurações de SerialPort e Processador).

Obs.: Utilizei para este projeto um arduino nano de processador ATmega328P e porta serial COM10.

Passo 3:
	Renomeie o arquivo de configuração do Counter-Strike Global Offensive (csgo-cfg/gamestate_integration_YOURNAME) e insira-o na lista de configs do seu jogo.

Exp.: gamestate_integration_minhaintegracao

Obs.: A pasta de configuração normalmente fica: Steam\steamapps\common\Counter-Strike Global Offensive\csgo\cfg
	  Só poderá ser alterado o "YOURNAME" do arquivo de configuração, a parte anterior é para o csgo indentificar como arquivo de integração.

Passo 4:
	Instale as dependências necessárias para o funcionamento do arquivo NodeJS.

Dependencias: NodeJS (https://nodejs.org/)
			  npm - Para efetuar a instalação do modulo posterior (npm é instalado junto com o NodeJS)
			  serialport (https://www.npmjs.com/package/serialport) Leia a descrição para entender como efetua a instalação pelo npm

Obs.: Para efetuar a instação do módulo "serialport", acesse o seu promp de comando e acesse a pasta onde está seu arquivo NodeJS (app.js).
	  Ao chegar na pasta requerida, utilize o seguinte comando: npm install serialport --save

Passo 5: Inicie o arquivo app.js pelo promp de comando indo até a pasta do arquivo e utlizando o seguinte comando: node app

Passo 6.1: Caso queira criar um arquivo para efetuar uma inicialização direta siga os próximos passos.

Passo 6.2: Crie um arquivo dentro da pasta onde está o arquivo app.js com o seguinte nome "run" e extensão "cmd" (run.cmd).

Passo 6.3: Abre-o com um editor de código de sua preferência, e coloque o seguinte código e estará pronto para ser iniciado:

@echo off

IF EXIST app.js (
	node app.js
)

ECHO ON
