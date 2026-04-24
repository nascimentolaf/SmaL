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
          SmaL Web
        </q-toolbar-title>
      <div> v1.0.1 </div>
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
    caption: 'Data Acquisition',
    icon: 'sensors',
    link: '/#/'
  },
  {
    title: 'Charts',
    caption: 'Charts Visualization',
    icon: 'line_axis',
    link: '/#/graficos'
  }
]

const linksList2 = [
  {
    title: 'Project Repository',
    caption: 'Github',
    icon: 'code',
    link: 'https://github.com/nascimentolaf/SmaL'
  },
  {
    title: 'Alaf Nascimento',
    caption: 'Author\'s Scholar Profile',
    icon: 'school',
    link: 'https://scholar.google.com/citations?user=FMMwyvIAAAAJ&hl=pt-BR&oi=ao'
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
