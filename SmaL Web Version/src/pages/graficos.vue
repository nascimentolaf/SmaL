<template>
<q-page class="row flex-center">
  <q-carousel
      v-model="slide"
      vertical
      transition-prev="slide-down"
      transition-next="slide-up"
      swipeable
      animated
      control-color="white"
      navigation-icon="radio_button_unchecked"
      navigation
      padding
      arrows
      responsive
      height="400px"
      class="text-white shadow-1 rounded-borders"
    >
      <q-carousel-slide name="style1" class="column no-wrap flex-center">
        <LineChartGenerator
          :chart-options="chartOptions"
          :chart-data="chartData[0]"
          :chart-id="chartId"
          :dataset-id-key="datasetIdKey"
          :plugins="plugins"
          :css-classes="cssClasses"
          :styles="styles"
        />
      </q-carousel-slide>
      <q-carousel-slide name="style2" class="column no-wrap flex-center">
        <LineChartGenerator
          :chart-options="chartOptions"
          :chart-data="chartData[1]"
          :chart-id="chartId"
          :dataset-id-key="datasetIdKey"
          :plugins="plugins"
          :css-classes="cssClasses"
          :styles="styles"
        />
      </q-carousel-slide>
      <q-carousel-slide name="style3" class="column no-wrap flex-center">
        <LineChartGenerator
          :chart-options="chartOptions"
          :chart-data="chartData[2]"
          :chart-id="chartId"
          :dataset-id-key="datasetIdKey"
          :plugins="plugins"
          :css-classes="cssClasses"
          :styles="styles"
        />
      </q-carousel-slide>
      <q-carousel-slide name="style4" class="column no-wrap flex-center">
        <LineChartGenerator
          :chart-options="chartOptions"
          :chart-data="chartData[3]"
          :chart-id="chartId"
          :dataset-id-key="datasetIdKey"
          :plugins="plugins"
          :css-classes="cssClasses"
          :styles="styles"
        />
      </q-carousel-slide>
      <q-carousel-slide name="style5" class="column no-wrap flex-center">
        <LineChartGenerator
          :chart-options="chartOptions"
          :chart-data="chartData[4]"
          :chart-id="chartId"
          :dataset-id-key="datasetIdKey"
          :plugins="plugins"
          :css-classes="cssClasses"
          :styles="styles"
        />
      </q-carousel-slide>
      <q-carousel-slide name="style6" class="column no-wrap flex-center">
        <LineChartGenerator
          :chart-options="chartOptions"
          :chart-data="chartData[5]"
          :chart-id="chartId"
          :dataset-id-key="datasetIdKey"
          :plugins="plugins"
          :css-classes="cssClasses"
          :styles="styles"
        />
      </q-carousel-slide>
    </q-carousel>

</q-page>
</template>

<script>
import { Line as LineChartGenerator } from 'vue-chartjs'
import { ref } from 'vue'

const parametros = ['Temperatura [°C]', 'Oximetria [%]', 'Frequência de Pulso [bpm]', 'Respiração [IRPM]', 'Frequência Cardíaca [bpm]', 'Pressão Não Invasiva [mmHg]']
const colors = ['red', 'blue', 'green', 'yellow', 'purple', 'orange']

function getLocalStorage (key) {
  return JSON.parse(window.sessionStorage.getItem(key))
}

const valueArrayValue = []
const valueArrayTime = []
let valueArrayValueAux = []
let valueArrayTimeAux = []

for (let i = 0; i < 6; i++) {
  const valueArrayHere = getLocalStorage('value' + (i + 1))

  if (valueArrayHere) {
    valueArrayTimeAux[0] = valueArrayHere[0].time
    valueArrayValueAux[0] = valueArrayHere[0].value

    for (let j = 1; j < valueArrayHere.length; j++) {
      valueArrayTimeAux[j] = valueArrayHere[j][0].time
      valueArrayValueAux[j] = valueArrayHere[j][0].value
    }

    valueArrayTime[i] = valueArrayTimeAux
    valueArrayValue[i] = valueArrayValueAux

    valueArrayValueAux = []
    valueArrayTimeAux = []
  }
}

import {
  Chart as ChartJS,
  Title,
  Tooltip,
  Legend,
  LineElement,
  LinearScale,
  CategoryScale,
  PointElement
} from 'chart.js'

ChartJS.register(
  Title,
  Tooltip,
  Legend,
  LineElement,
  LinearScale,
  CategoryScale,
  PointElement
)

export default {
  name: 'LineChart',
  setup () {
    return {
      slide: ref('style1'),
      width: 900,
      height: 300
    }
  },
  components: {
    LineChartGenerator
  },
  props: {
    chartId: {
      type: String,
      default: 'line-chart'
    },
    datasetIdKey: {
      type: String,
      default: 'label'
    },
    cssClasses: {
      default: '',
      type: String
    },
    styles: {
      type: Object,
      default: () => {}
    },
    plugins: {
      type: Array,
      default: () => []
    }
  },
  data () {
    return {
      parametros: ['Temperatura [°C]', 'Oximetria [%]', 'Frequência de Pulso [bpm]', 'Respiração [IRPM]', 'Frequência Cardíaca [bpm]', 'Pressão Não Invasiva [mmHg]'],

      chartData: [{
        labels: valueArrayTime[0],
        datasets: [
          {
            label: parametros[0],
            backgroundColor: colors[0],
            data: valueArrayValue[0]
          }
        ]
      },
      {
        labels: valueArrayTime[1],
        datasets: [
          {
            label: parametros[1],
            backgroundColor: colors[1],
            data: valueArrayValue[1]
          }
        ]
      },
      {
        labels: valueArrayTime[2],
        datasets: [
          {
            label: parametros[2],
            backgroundColor: colors[2],
            data: valueArrayValue[2]
          }
        ]
      },
      {
        labels: valueArrayTime[3],
        datasets: [
          {
            label: parametros[3],
            backgroundColor: colors[3],
            data: valueArrayValue[3]
          }
        ]
      },
      {
        labels: valueArrayTime[4],
        datasets: [
          {
            label: parametros[4],
            backgroundColor: colors[4],
            data: valueArrayValue[4]
          }
        ]
      },
      {
        labels: valueArrayTime[5],
        datasets: [
          {
            label: parametros[5],
            backgroundColor: colors[5],
            data: valueArrayValue[5]
          }
        ]
      }],
      chartOptions: {
        responsive: true,
        maintainAspectRatio: false
      }
    }
  }
}
</script>
