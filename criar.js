const fs = require("fs");
const readline = require("readline");
const { google } = require("googleapis");
var moment = require("moment");
// const show = require("./show.js");
const meow = require("meow");
const cli = meow("Usage");
var input = cli.input;
// If modifying these scopes, delete token.json.
const SCOPES = ["https://www.googleapis.com/auth/calendar"];
const TOKEN_PATH = "token.json";
var calendar;
let time0;
var events

// Load client secrets from a local file.
fs.readFile("credentials.json", (err, content) => {
  if (err) return console.log("Error loading client secret file:", err);
  // Authorize a client with credentials, then call the Google Calendar API.
  authorize(JSON.parse(content), main);
});

/**
 * Create an OAuth2 client with the given credentials, and then execute the
 * given callback function.
 * @param {Object} credentials The authorization client credentials.
 * @param {function} callback The callback to call with the authorized client.
 */
function authorize(credentials, callback) {
  const { client_secret, client_id, redirect_uris } = credentials.installed;
  const oAuth2Client = new google.auth.OAuth2(
    client_id,
    client_secret,
    redirect_uris[0]
  );

  // Check if we have previously stored a token.
  fs.readFile(TOKEN_PATH, (err, token) => {
    if (err) return getAccessToken(oAuth2Client, callback);
    oAuth2Client.setCredentials(JSON.parse(token));
    callback(oAuth2Client);
  });
}

/**
 * Get and store new token after prompting for user authorization, and then
 * execute the given callback with the authorized OAuth2 client.
 * @param {google.auth.OAuth2} oAuth2Client The OAuth2 client to get token for.
 * @param {getEventsCallback} callback The callback for the authorized client.
 */
function getAccessToken(oAuth2Client, callback) {
  const authUrl = oAuth2Client.generateAuthUrl({
    access_type: "offline",
    scope: SCOPES
  });
  console.log("Authorize this app by visiting this url:", authUrl);
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
  });
  rl.question("Enter the code from that page here: ", code => {
    rl.close();
    oAuth2Client.getToken(code, (err, token) => {
      if (err) return console.error("Error retrieving access token", err);
      oAuth2Client.setCredentials(token);
      // Store the token to disk for later program executions
      fs.writeFile(TOKEN_PATH, JSON.stringify(token), err => {
        if (err) console.error(err);
        console.log("Token stored to", TOKEN_PATH);
      });
      callback(oAuth2Client);
    });
  });
}

/**
 * Lists the next 10 events on the user's primary calendar.
 * @param {google.auth.OAuth2} auth An authorized OAuth2 client.
 */
function cale(auth) {
  calendar = google.calendar({ version: "v3", auth });
}

function createEvents(auth, starttime, endtime) {
  // cale(auth)
  const timeZone = "America/Bahia";
  var event = {
    summary: input[0],
    location: "Tiradentes ->  SALA X",
    description: "SALA AGENDADA PELO 'PLOT'",
    start: {
      dateTime: starttime,
      timeZone: timeZone
    },
    end: {
      dateTime: endtime,
      timeZone: timeZone
    },
    recurrence: ["RRULE:FREQ=DAILY;COUNT=1"],
    attendees: [{ email: input[0] }],
    reminders: {
      useDefault: false,
      overrides: [
        { method: "email", minutes: 24 * 60 },
        { method: "popup", minutes: 10 }
      ]
    }
  };
  calendar.events.insert(
    {
      auth: auth,
      calendarId: "primary",
      resource: event
    },
    function(err, event) {
      if (err) {
        console.log(
          "There was an error contacting the Calendar service: " + err
        );
        return;
      }

      console.log("Event created: %s", event.htmlLink);
    }
  );
}

function listEvents() {
  // const calendar = google.calendar({ version: "v3", auth });
  time0 = null;
  calendar.events.list(
    {
      calendarId: "primary",
      timeMin: new Date().toISOString(),
      maxResults: 10,
      singleEvents: true,
      orderBy: "startTime"
    },
    (err, res) => {
      if (err) return console.log("The API returned an error: " + err);
      events = res.data.items;
      if (events.length) {
        //  console.log("Upcoming 10 events:");
        time0 = res.data.items[0].start.dateTime;
        console.log(time0);
        events.map((event, i) => {
          const start = event.start.dateTime || event.start.date;
          // console.log(`${start} - ${event.summary}`);
        });
      } else {
        console.log("No upcoming events found.");
      }
    }
  );
}

function main(auth) {
  cale(auth);
  var starttime = moment();
  var endtime = starttime.clone().add(1, "hour");
  listEvents();
  console.log("eeeee", time0);
  // console.log("lool", endtime.diff(time0, "seconds"));
  if (endtime.diff(time0, "seconds") < 0) {
    // console.log("b");
    endtime = time0;
  }
  starttime.format();
  // console.log(starttime);
  endtime.format();
  // console.log(endtime);
  console.log(input);
  createEvents(auth, starttime, endtime);
}
