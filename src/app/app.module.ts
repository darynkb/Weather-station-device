import { HttpClientModule } from '@angular/common/http';
import { NgModule } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { BrowserModule } from '@angular/platform-browser';
import { AppComponent } from './app.component';
import { TemphumService } from './temphum.service';
import { BrowserAnimationsModule } from "@angular/platform-browser/animations";
//import { CategoryChartLineChartMultipleSourcesComponent } from "./line-chart-multiple-sources/category-chart-line-chart-multiple-sources.component";
import { 
	IgxCategoryChartModule,
	IgxLegendModule
 } from "igniteui-angular-charts";
import { NgApexchartsModule } from 'ng-apexcharts';

@NgModule({
  declarations: [
    AppComponent,
  ],
  imports: [
    BrowserModule, HttpClientModule, FormsModule,IgxCategoryChartModule,IgxLegendModule,ReactiveFormsModule, NgApexchartsModule,],
  providers: [TemphumService],
  bootstrap: [AppComponent]
})
export class AppModule { }
