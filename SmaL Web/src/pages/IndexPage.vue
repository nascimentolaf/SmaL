<template>
<q-page class="row q-ma-sm">
<div class="row" style="margin-top: auto; margin-bottom: auto; margin-left: auto;">
    <td class='text-right' >
      <q-btn
        color='primary'
        label='Selecionar Porta'
        no-caps
        @click='created'
      />
    </td>
</div>

    <div class="col-12">
      <div class="row">
      <div class="col-xs-12 col-sm-6 col-md-3 q-pa-sm">
        <CardBadge
          title='Temperatura'
          borderColor="red"
          color="red">
          <template v-slot:content>
            <div class="text-h5 text-white">
              {{ getValue1 }}°C
            </div>
          </template>
        </CardBadge>
      </div>

      <div class="col-xs-12 col-sm-6 col-md-3 q-pa-sm">
      <CardBadge
        title='Oximetria'
        borderColor="green"
        color="blue">
        <template v-slot:content>
          <div class="text-h5 text-white">
            {{ getValue2 }} %
          </div>
        </template>
      </CardBadge>
      </div>

      <div class="col-xs-12 col-sm-6 col-md-3 q-pa-sm">
      <CardBadge
        title='Frequência de Pulso'
        borderColor="green"
        color="green">
        <template v-slot:content>
          <div class="text-h5 text-white">
            {{ getValue3 }} bpm
          </div>
        </template>
      </CardBadge>
      </div>

      <div class="col-xs-12 col-sm-6 col-md-3 q-pa-sm">
      <CardBadge
        title='Respiração'
        borderColor="yellow"
        color="yellow">
        <template v-slot:content>
          <div class="text-h5 text-white">
            {{ getValue4 }} IRPM
          </div>
        </template>
      </CardBadge>
      </div>

      <div class="col-xs-12 col-sm-6 col-md-3 q-pa-sm">
      <CardBadge
        title='Frequência Cardíaca'
        borderColor="purple"
        color="purple">
        <template v-slot:content>
          <div class="text-h5 text-white">
            {{ getValue5 }} bpm
          </div>
        </template>
      </CardBadge>
      </div>

      <div class="col-xs-12 col-sm-6 col-md-3 q-pa-sm">
      <CardBadge
        title='Pressão Não Invasiva'
        borderColor="orange"
        color="orange">
        <template v-slot:content>
          <div class="text-h5 text-white">
            {{ getValue6 }} mmHg
          </div>
        </template>
      </CardBadge>
      </div>

  <div class="col-12 justify-center flex" style="margin-top:10px;">
      Última Leitura: {{leitura}}
  </div>

  </div>
  </div>
  </q-page>
</template>

<script>
import CardBadge from 'components/cardBadge'

function setLocalStorage (key, value) {
  window.sessionStorage.setItem(key, JSON.stringify(value))
}

function getLocalStorage (key) {
  return JSON.parse(window.sessionStorage.getItem(key))
}

function sleep (ms) {
  return new Promise(resolve => setTimeout(resolve, ms))
}

function floatI3E754 (bitString) {
  let value = 0
  let sign = 1

  if (bitString[0] === '1') sign = -1

  let exp754 = parseInt(bitString[1]) * Math.pow(2, 7) +
                    parseInt(bitString[2]) * Math.pow(2, 6) +
                    parseInt(bitString[3]) * Math.pow(2, 5) +
                    parseInt(bitString[4]) * Math.pow(2, 4) +
                    parseInt(bitString[5]) * Math.pow(2, 3) +
                    parseInt(bitString[6]) * Math.pow(2, 2) +
                    parseInt(bitString[7]) * Math.pow(2, 1) +
                    parseInt(bitString[8]) * Math.pow(2, 0)

  exp754 = exp754 - 127

  let power = -1
  let mant = 0.0

  for (let i = 0; i < 23; i++) {
    const c = bitString[i + 9]
    mant += c * Math.pow(2.0, power)
    power--
  }
  mant += 1.0

  value = (sign * Math.pow(2.0, exp754) * mant) / 10 // mais robusto - valores entre 0 e 2
  return value.toFixed(2)
}

export default {
  name: 'IndexPage',
  data () {
    return {
      dataValue: [0, 0, 0, 0, 0, 0],
      leitura: 'nenhum dado recebido'
    }
  },
  components: {
    CardBadge
  },
  computed: {
    getValue1 () {
      return `${this.dataValue[0]}`
    },
    getValue2 () {
      return `${this.dataValue[1]}`
    },
    getValue3 () {
      return `${this.dataValue[2]}`
    },
    getValue4 () {
      return `${this.dataValue[3]}`
    },
    getValue5 () {
      return `${this.dataValue[4]}`
    },
    getValue6 () {
      return `${this.dataValue[5]}`
    }
  },
  setup () {
  },

  methods: {
    async handleData () {
      if ('serial' in navigator) {
        let lastTime1 = new Date()
        let lastTime2 = new Date()
        let lastTime3 = new Date()
        let lastTime4 = new Date()
        let lastTime5 = new Date()
        let lastTime6 = new Date()

        // The Web Serial API is supported.
        const port = await navigator.serial.requestPort() // Prompt user to select any serial port.
        await port.open({ baudRate: 115200 })
        await sleep(500)

        while (port.readable) {
          const reader = port.readable.getReader()

          try {
            while (true) {
              const { value, done } = await reader.read()
              if (value) {
                const nPayloads = 6 // 6 variables cada
                const nBytes = 4 // 4 bytes
                const bitString = []

                for (let i = 0; i < nBytes * nPayloads; i += nBytes) {
                  const value32 = (value[i] << 24) + (value[i + 1] << 16) + (value[i + 2] << 8) + value[i + 3] // repensar talvez

                  // tratar no futuro
                  if (value32 >= 0) {
                    bitString[i / nBytes] = value32.toString(2)
                  }

                  // se valor positivo
                  if (bitString[i / nBytes].length === 31) {
                    bitString[i / nBytes] = '0' + bitString[i / nBytes]
                  }

                  const valueHere = floatI3E754(bitString[i / nBytes])

                  const index = (i / nBytes) + 1
                  const idHere = 'value' + index

                  if (!isNaN(valueHere)) {
                    const timeNow1 = new Date()
                    const timeNow2 = new Date()
                    const timeNow3 = new Date()
                    const timeNow4 = new Date()
                    const timeNow5 = new Date()
                    const timeNow6 = new Date()

                    this.leitura = new Date()

                    const periodoAmostragem = 0.3 // em segundos

                    if ((idHere === 'value1') && (timeNow1.getTime() - lastTime1.getTime() >= periodoAmostragem * 1000)) {
                      this.dataValue[0] = verifyTime(valueHere)
                      storageData(idHere, this.dataValue[0])
                      lastTime1 = timeNow1
                    }

                    if ((idHere === 'value2') && (timeNow2.getTime() - lastTime2.getTime() >= periodoAmostragem * 1000)) {
                      this.dataValue[1] = verifyTime(valueHere)
                      storageData(idHere, this.dataValue[1])
                      lastTime2 = timeNow2
                    }

                    if ((idHere === 'value3') && (timeNow3.getTime() - lastTime3.getTime() >= periodoAmostragem * 1000)) {
                      this.dataValue[2] = verifyTime(valueHere)
                      storageData(idHere, this.dataValue[2])
                      lastTime3 = timeNow3
                    }

                    if ((idHere === 'value4') && (timeNow4.getTime() - lastTime4.getTime() >= periodoAmostragem * 1000)) {
                      this.dataValue[3] = verifyTime(valueHere)
                      storageData(idHere, this.dataValue[3])
                      lastTime4 = timeNow4
                    }

                    if ((idHere === 'value5') && (timeNow5.getTime() - lastTime5.getTime() >= periodoAmostragem * 1000)) {
                      this.dataValue[4] = verifyTime(valueHere)
                      storageData(idHere, this.dataValue[4])
                      lastTime5 = timeNow5
                    }

                    if ((idHere === 'value6') && (timeNow6.getTime() - lastTime6.getTime() >= periodoAmostragem * 1000)) {
                      this.dataValue[5] = verifyTime(valueHere)
                      storageData(idHere, this.dataValue[5])
                      lastTime6 = timeNow6
                    }

                    function verifyTime (valueHere) {
                      if (valueHere === '999.00') { return '- ? -' }
                      return valueHere
                    }

                    function storageData (index, valueNow) {
                      if (valueNow !== '- ? -') {
                        const valueStorage = getLocalStorage(index)
                        let timeHere = new Date()
                        let hours = timeHere.getHours()
                        let minutes = timeHere.getMinutes()
                        let seconds = timeHere.getSeconds()

                        if (hours <= 9) hours = '0' + hours
                        if (minutes <= 9) minutes = '0' + minutes
                        if (seconds <= 9) seconds = '0' + seconds

                        timeHere = hours + ':' + minutes + ':' + seconds

                        const valueJSON = [{
                          time: timeHere,
                          value: parseFloat(valueNow).toFixed(2)
                        }]

                        if (valueStorage) {
                          valueStorage.push(valueJSON)
                          setLocalStorage(index, valueStorage)
                        } else { setLocalStorage(index, valueJSON) }
                      }
                    }
                  }
                }
              }
              if (done) {
                reader.releaseLock()
                break
              }
            }
          } catch (error) {
            // console.error(error);
          } finally {
            reader.releaseLock()
          }
        }
        await port.close()
      }
    },
    async created () {
      this.handleData()
    }
  }
}
</script>
