const SerialPort = require("serialport"),
      Readline   = SerialPort.parsers.Readline,
      sPort      = new SerialPort("COM10", {baudRate: 9600}),
      parser     = new Readline(),
      Arduino    = {},
      http       = require('http'),
      fs         = require('fs'),
      port       = 3000,
      host       = 'localhost';

let csgo         = false,
    timerCs      = 0,
    team         = null,
    clearTeam    = false,
    redHealth    = false,
    flashed      = false;

sPort.pipe(parser)

parser.on("data", (data) =>
{
    data = JSON.parse(data);
    if(data.arduino)
        console.log("\x1b[32m%s\x1b[0m", "Arduino Start");
});

server = http.createServer((req, res) =>
{
    if (req.method == 'POST')
    {
        res.writeHead(200, {'Content-Type': 'text/html'});

        var body = '';
        req.on('data', (data) =>
        {
            body += data;
        });
        req.on('end', () =>
        {
            body = JSON.parse(body);

            // console.log(body);
            if(body.provider.appid == 730)
            {
                timerCs = Date.now();
                if(!csgo)
                    setTimeout(() =>{Arduino.SetUseApp(true);}, 10);
            }

            if(body.player && body.player.state && body.player.state.health > 20 && redHealth)
            {
                redHealth = false;
                Arduino.SetHealth(redHealth);
            }

            if(body.player && body.player.state && body.player.state.flashed == 0 && flashed)
                flashed = false;


            if(!body.player.team && team)
            {
                team = null;
                clearTeam = false;
            }
            else if(team != body.player.team)
            {
                team = body.player.team;
                clearTeam = false;
                Arduino.SetTeam(team);
            }
            else if(body.player && body.player.team && body.round && body.round.phase != "freezetime" && body.map && body.map.round == 0 && !clearTeam)
            {
                clearTeam = true;
                Arduino.SetTeam();
            }
            else if(body.player && body.player.team && body.player.state && body.player.state.health > 0 && body.player.state.health <= 20 && !redHealth)
            {
                redHealth = true;
                Arduino.SetHealth(redHealth);
            }
            else if(body.player && body.player.team && body.player.state && body.player.state.flashed > 0 && !flashed)
            {
                flashed = true;
                Arduino.SetFlash(body.player.state.flashed);
            }

            res.end('');
        });
    }
    else
    {
        console.log("Not expecting other request types...");
        res.writeHead(200, {'Content-Type': 'text/html'});
        let html = '<html><body>HTTP Server at http://' + host + ':' + port + '</body></html>';
        res.end(html);
    }

});

server.listen(port, host);
console.log("\x1b[32m%s\x1b[0m", "Create Server: " + host + ":" + port);

Arduino.SetUseApp = (bool) =>
{
    sPort.write(`app=${bool}`);
}

Arduino.SetData = (parm1, parm2, parm3, parm4, parm5, parm6, parm7, parm8) =>
{
    sPort.write(`data=${parm1};${parm2};${parm3};${parm4};${parm5};${parm6};${parm7};${parm8}`);
}

Arduino.SetTeam = (team) =>
{
    if(team == "CT")
        Arduino.SetData(false, 1, 100, 4, 0, 0, 255, 98);
    else if(team == "T")
        Arduino.SetData(false, 1, 100, 4, 255, 0, 0, 98);
    else
        Arduino.SetData(false, 1, 100, 1, 0, 0, 0, 100);
}

Arduino.SetHealth = (health) =>
{
    if(health)
        Arduino.SetData(false, 1, 100, 5, 255, 0, 0, 99);
    else
        Arduino.SetData(false, 1, 100, 1, 0, 0, 0, 100);
}

Arduino.SetFlash = (flash) =>
{
    Arduino.SetData(false, 1, 100, 6, flash, flash, flash, 50);
}

setInterval(() =>
{
    if(Date.now() - timerCs > 18000)
    {
        if(csgo)
        {
            csgo = false;
            console.log("Fechou o Counter-Striker");
            Arduino.SetUseApp(false);
        }
    }
    else
    {
        if(!csgo)
        {
            csgo = true;
            console.log("Abriu o Counter-Striker");
        }
        Arduino.SetUseApp(true);
    }
}, 15000);