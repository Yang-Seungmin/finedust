// Set new default font family and font color to mimic Bootstrap's default styling
Chart.defaults.global.defaultFontFamily = '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
Chart.defaults.global.defaultFontColor = '#292b2c';

// Area Chart Example
var pm10 = document.getElementById("nodePM10");
var pm25 = document.getElementById("nodePM25");

var denyButton = document.getElementById("buttonDeny");

var labels = ["04:53:22", "04:54:24", "04:55:30", "04:56:32", "04:57:34", "04:58:36", "04:59:40", "05:00:42", "05:01:44", "05:02:46", "05:03:50", "05:04:53", "05:05:55", "05:06:57", "05:08:01", "05:09:03", "05:10:05", "05:11:09", "05:12:11", "05:13:13"];
var value_pm10 = [2.00, 1.40, 1.90, 1.70, 1.70, 1.60, 1.80, 1.20, 2.80, 1.40, 3.30, 2.10, 5.20, 1.20, 1.70, 1.70, 2.20, 4.00, 3.00, 1.50];
var value_pm25 = [1.50, 1.40, 1.80, 1.60, 1.60, 1.50, 1.70, 1.20, 2.10, 1.30, 2.60, 1.90, 1.50, 1.20, 1.60, 1.60, 2.00, 1.70, 1.70, 1.50];

var pm10Max = 0;
var pm25Max = 0;

for(var i = 0; i < value_pm10.length; i++) {
  if(pm10Max < value_pm10[i]) pm10Max = value_pm10[i];
}

for(var i = 0; i < value_pm25.length; i++) {
  if(pm25Max < value_pm25[i]) pm25Max = value_pm25[i];
}

denyButton.onclick = ev => {

};


var pm10Chart = new Chart(pm10, {
  type: 'line',
  data: {
    labels: labels,
    datasets: [{
      label: "PM10",
      lineTension: 0.3,
      backgroundColor: "rgba(2,117,216,0.2)",
      borderColor: "rgba(2,117,216,1)",
      pointRadius: 5,
      pointBackgroundColor: "rgba(2,117,216,1)",
      pointBorderColor: "rgba(255,255,255,0.8)",
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(2,117,216,1)",
      pointHitRadius: 50,
      pointBorderWidth: 2,
      data: value_pm10,
    }],
  },
  options: {
    scales: {
      xAxes: [{
        time: {
          unit: 'date'
        },
        gridLines: {
          display: false
        },
        ticks: {
          maxTicksLimit: 7
        }
      }],
      yAxes: [{
        ticks: {
          min: 0,
          max: pm10Max * 1.5,
          maxTicksLimit: 5
        },
        gridLines: {
          color: "rgba(0, 0, 0, .125)",
        }
      }],
    },
    legend: {
      display: false
    }
  }
});

var pm25Chart = new Chart(pm25, {
  type: 'line',
  data: {
    labels: labels,
    datasets: [{
      label: "PM2.5",
      lineTension: 0.3,
      backgroundColor: "rgba(220,53,69,0.2)",
      borderColor: "rgba(220,53,69,1)",
      pointRadius: 5,
      pointBackgroundColor: "rgba(220,53,69,1)",
      pointBorderColor: "rgba(255,255,255,0.8)",
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(220,53,69,1)",
      pointHitRadius: 50,
      pointBorderWidth: 2,
      data: value_pm25,
    }],
  },
  options: {
    scales: {
      xAxes: [{
        time: {
          unit: 'date'
        },
        gridLines: {
          display: false
        },
        ticks: {
          maxTicksLimit: 7
        }
      }],
      yAxes: [{
        ticks: {
          min: 0,
          max: pm25Max * 1.5,
          maxTicksLimit: 5
        },
        gridLines: {
          color: "rgba(0, 0, 0, .125)",
        }
      }],
    },
    legend: {
      display: false
    }
  }
});
