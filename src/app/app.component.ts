import { HttpErrorResponse } from '@angular/common/http';
import { Component, OnInit,ViewChild } from '@angular/core';
import { Temphum } from './temphum';
import { TemphumService } from './temphum.service';

import {
  ChartComponent,
  ApexAxisChartSeries,
  ApexChart,
  ApexXAxis,
  ApexDataLabels,
  ApexTitleSubtitle,
  ApexStroke,
  ApexGrid
} from "ng-apexcharts";
import { range } from 'list';

export type ChartOptions = {
  series: ApexAxisChartSeries;
  chart: ApexChart;
  xaxis: ApexXAxis;
  dataLabels: ApexDataLabels;
  grid: ApexGrid;
  stroke: ApexStroke;
  title: ApexTitleSubtitle;
};


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css','./css/style.css','lib/flaticon/font/flaticon.css','lib/owlcarousel/assets/owl.carousel.min.css','lib/tempusdominus/css/tempusdominus-bootstrap-4.min.css']
})
export class AppComponent implements OnInit{
  

  @ViewChild("chart",{static:false}) chart : ChartComponent;
  public chartOptions: Partial<ChartOptions> | any;
  public chartOptions2: Partial<ChartOptions> | any;

  [x: string]: any;
  public temphums: Temphum[];
  public data: any[];
  public _series: {name: string, data: number[]}[];
  public name: String;
  public temps: number[] = this.getTemps();
  public hums: number[] = this.getHums();
  public indexes: number[] = this.getIndexes();
  // public temps2: number[] = this.getTemps2();
  // public hums2: number[] = this.getHums2();
  // public indexes2: number[] = this.getIndexes2();
  public div1: boolean=false;
  public div2: boolean=false;

  constructor(private temphumService: TemphumService){
    
    this.chartOptions = {
      series: [
      ],
      chart: {
        height: 350,
        type: "line",
        zoom: {
          enabled: false
        }
      },
      dataLabels: {
        enabled: false
      },
      stroke: {
        curve: "straight"
      },
      title: {
        text: "Temperature Humidity Records",
        align: "center"
      },
      grid: {
        row: {
          colors: ["#f3f3f3", "transparent"], // takes an array which will be repeated on columns
          opacity: 0.5
        }
      },
      xaxis: {
        categories: 0
      }
    }    
    // this.chartOptions2 = {
    //   series: [
    //   ],
    //   chart: {
    //     height: 350,
    //     type: "line",
    //     zoom: {
    //       enabled: false
    //     }
    //   },
    //   dataLabels: {
    //     enabled: false
    //   },
    //   stroke: {
    //     curve: "straight"
    //   },
    //   title: {
    //     text: "Temperature Humidity Records",
    //     align: "center"
    //   },
    //   grid: {
    //     row: {
    //       colors: ["#f3f3f3", "transparent"], // takes an array which will be repeated on columns
    //       opacity: 0.5
    //     }
    //   },
    //   xaxis: {
    //     categories: 0
    //   }
    // }  
  }
  
  public updateSeries() {
    this.chartOptions.series = [
      {
        name: "Humidity",
        data: this.hums
      },
      {
        name: "Temperature",
        data: this.temps
      }];
    this.chartOptions.xaxis.categories = this.indexes

    // this.chartOptions2.series = [
    //   {
    //     name: "Humidity",
    //     data: this.hums2
    //   },
    //   {
    //     name: "Temperature",
    //     data: this.temps2
    //   }];
    // this.chartOptions2.xaxis.categories = this.indexes2
  }


  async ngOnInit() {
    function delay(ms: number) {
    return new Promise( resolve => setTimeout(resolve, ms) );
  }
    await delay(1000);
    
    this.updateSeries();
    
  }

  public getTemphums(): void{
    this.temphumService.getTempHums().subscribe(
      (response: Temphum[])=>{
        this.temphums = response;
      }
    )
  }

  div1Function(){

    this.div1=!this.div1;
}

// div2Function(){

//   this.div2=!this.div2;
// }
  public getTemps(): number[]{
    let num: number[]=[];
    this.temphumService.getTemps().subscribe(
        (response: number[]) => {
          response.forEach(element => {
            num.push(element);
          });
        }
    )
    return num;
  }

  public getHums(): number[]{
    let num: number[]=[];
    this.temphumService.getHums().subscribe(
        (response: number[]) => {
          response.forEach(element => {
            num.push(element);
          });
        }
    )
    return num;
  }

  public getIndexes(): number[]{
    let num: number[]=[];
    this.temphumService.getIndexes().subscribe(
        (response: number[]) => {
          response.forEach(element => {
            num.push(element);
          });
        }
    )
    return num;
  }

  // public getTemps2(): number[]{
  //   let num: number[]=[];
  //   this.temphumService.getTemps2().subscribe(
  //       (response: number[]) => {
  //         response.forEach(element => {
  //           num.push(element);
  //         });
  //       }
  //   )
  //   return num;
  // }

  // public getHums2(): number[]{
  //   let num: number[]=[];
  //   this.temphumService.getHums2().subscribe(
  //       (response: number[]) => {
  //         response.forEach(element => {
  //           num.push(element);
  //         });
  //       }
  //   )
  //   return num;
  // }

  // public getIndexes2(): number[]{
  //   let num: number[]=[];
  //   this.temphumService.getIndexes2().subscribe(
  //       (response: number[]) => {
  //         response.forEach(element => {
  //           num.push(element);
  //         });
  //       }
  //   )
  //   return num;
  // }
}
