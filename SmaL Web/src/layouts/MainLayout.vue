<template>
  <q-layout view="lHh Lpr lff">
    <q-header elevated>
      <q-toolbar>
        <q-btn
          flat
          dense
          round
          icon="menu"
          aria-label="Menu"
          @click="toggleLeftDrawer"
        />
        <q-toolbar-title>
          SmaL Web Version
        </q-toolbar-title>
      <div> v1.0.0 </div>
      </q-toolbar>
    </q-header>

    <q-drawer
      v-model="leftDrawerOpen"
      show-if-above
      bordered
    >
      <q-list>
      <br>
        <EssentialLink
          v-for="link in essentialLinks"
          :key="link.title"
          v-bind="link"
        />
      </q-list>

      <br><br>
      <q-item-label header>
        Veja Também
      </q-item-label>
      <q-list>
      <br>
        <EssentialLink2
          v-for="link in essentialLinks2"
          :key="link.title"
          v-bind="link"
        />
      </q-list>
    </q-drawer>

    <q-footer  class="bg-white justify-center flex">
        <img rel="icon" type="image/png" src="assets/3.png">
        <img rel="icon" type="image/png" src="assets/4.png">
        <img rel="icon" type="image/png" src="assets/2.png">
        <img rel="icon" type="image/png" src="assets/1.png">
        <img rel="icon" type="image/png" src="assets/5.png">
        <img rel="icon" type="image/png" src="assets/6.png">
        <img rel="icon" type="image/png" src="assets/9.png">
        <img rel="icon" type="image/png" src="assets/7.png">
        <img rel="icon" type="image/png" src="assets/8.png">
    </q-footer>

    <q-page-container>
      <router-view />
    </q-page-container>
  </q-layout>
</template>

<script>
import Userfront from '@userfront/core'
import { defineComponent, ref } from 'vue'
import EssentialLink from 'components/EssentialLink.vue'
import EssentialLink2 from 'components/EssentialLink2.vue'

Userfront.init('alaf')

const linksList = [
  {
    title: 'Home',
    caption: 'Recepção de Dados',
    icon: 'sensors',
    link: '/#/'
  },
  {
    title: 'Gráficos',
    caption: 'Visualização de Gráficos',
    icon: 'line_axis',
    link: '/#/graficos'
  }
]

const linksList2 = [
  {
    title: 'Alaf do Nascimento Santos',
    caption: 'Aluno',
    icon: 'school',
    link: 'https://scholar.google.com/citations?user=FMMwyvIAAAAJ&hl=pt-BR&oi=ao'
  },
  {
    title: 'Jair Adriano Lima Silva',
    caption: 'Orientador',
    icon: 'school',
    link: 'https://scholar.google.com/citations?user=QzvTJn8AAAAJ&hl=pt-BR&oi=ao'
  },
  {
    title: 'Higor Araújo Fim Camporez',
    caption: 'Coorientador',
    icon: 'school',
    link: 'https://scholar.google.com/citations?hl=pt-BR&user=mudtiQEAAAAJ'
  },
  {
    title: 'Repositório do Projeto',
    caption: 'Github',
    icon: 'code',
    link: 'https://github.com/alafSantos'
  }
]

export default defineComponent({
  name: 'MainLayout',

  components: {
    EssentialLink,
    EssentialLink2
  },

  setup () {
    const leftDrawerOpen = ref(false)

    return {
      url: 'assets/entidades.png',
      essentialLinks: linksList,
      essentialLinks2: linksList2,
      leftDrawerOpen,
      toggleLeftDrawer () {
        leftDrawerOpen.value = !leftDrawerOpen.value
      }
    }
  }
})
</script>
